package bgu.spl.net.impl.stomp;



import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.api.StompMessagingProtocol;



public class StompProtocolIMP implements StompMessagingProtocol<frameObject>{

    private boolean shouldTerminate = false;
    int connectionId; 
    ConnectionIMP<frameObject> connections;
    dataBase dataBase;

    @Override
    public void start(int connectionId, ConnectionIMP<frameObject> connections, dataBase dataBase){
        this.connectionId = connectionId;
        this.connections = connections;
        this.dataBase = dataBase;
    }

    @Override
    public void process(frameObject message) {
        //check if framObject is valide
        //frameObject message = ((frameObject)msg);
        System.out.println("the massege that recived from clien\n" + message.frameObjectToString());
        String commandLine =  message.commandLine;
        if(commandLine.equals("CONNECT")){
            connect(message);
        }
        else if(commandLine.equals("SEND")){
            send(message);
        }
        else if(commandLine.equals("SUBSCRIBE")){
            subscribe(message);
        }
        else if(commandLine.equals("UNSUBSCRIBE")){
            unsubscribe(message);
        }
        else if(commandLine.equals("DISCONNECT")){
            System.out.println("trying to disconnect");
            disconnect(message);
        }
        else{
            error(message,"invalid command");
        }
        
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }




    private void connect(frameObject message){
        String login = message.headers.get("login");
        String passcode = message.headers.get("passcode");
        if(!(message.headers.get("accept-version").equals("1.2"))
            ||!(message.headers.get("host").equals("stomp.cs.bgu.ac.il"))){
                error(message,"accept-version or host " + message.headers.get("host") + " or invalid " + message.headers.get("accept-version"));
            }
        else if(login == null || passcode == null){
            error(message,"login or passcode missing");
        }    
        else if(dataBase.managedToConnect(login,passcode,connectionId)){
            //new user join or old user
            //send a connect frame
            Map<String,String> headers = new HashMap<String,String>();
            headers.put("version", "1.2");
            frameObject connected = new frameObject("CONNECTED", headers, "");
            connections.send(connectionId, connected);
            receipt(message);
           
        }
        else{
            error(message,"passcode invalid or user alredy active");
        } 
         
    }


    private void send(frameObject message){
        String destination = message.headers.get("destination");
        if(destination == null)
            error(message, "malformed frame received - destination");
        else{
            ConcurrentHashMap<Integer,String> cannelCIdSId = dataBase.channelsMap.get(destination);
            if(cannelCIdSId == null){
                error(message, "destination doesnot exist");
            }
            else if(cannelCIdSId.get(connectionId) != null){
                //send a MESSAGE frame
                for(Map.Entry<Integer,String> cIdsId: cannelCIdSId.entrySet()){
                    Map<String,String> headers = new HashMap<String,String>();
                    headers.put("destination", destination);
                    headers.put("message-id", cIdsId.getValue());
                    Integer msgId = dataBase.getMessageId();
                    headers.put("message-id", msgId.toString());
                    connections.send(cIdsId.getKey(), new frameObject("MESSAGE", headers, message.body));
                }
            }
            else{
                error(message, "not subscribed to destination");
            }
        }
                            
    }
    private void subscribe(frameObject message){
        String destination = message.headers.get("destination");
        String id = message.headers.get("id");
        if(destination == null || id == null){
            error(message, "malformed frame received - destination/id");
        }
        else if(dataBase.subscribe(connectionId,id,destination)){
            //handle a SUBSCRIBE frame
            receipt(message);
        }
        else{
            error(message, "user alredy subscribed to destination");
        }

    }
    private void unsubscribe(frameObject message){
        String id = message.headers.get("id");
        if(id == null){
            error(message, "malformed frame received - id");
        }
        else if(dataBase.unsubscribe(id,connectionId)){
            //handle a UNSUBSCRIBE frame
            receipt(message);
        }
        else{
            error(message, "subscription ID dosent match");
        }

    }
    private void disconnect(frameObject message){
        String receipt = message.headers.get("receipt");
        if(receipt == null){
            error(message, "malformed frame received - receipt");
        }
        else{
            receipt(message);
            dataBase.disconnect(connectionId);
            connections.disconnect(connectionId);
            
        }
        shouldTerminate = true;
        System.out.println("succses to logout");

    }
    private void error(frameObject message,String error){
        Map<String,String> headers = new HashMap<String,String>();
        headers.put("message", error);
        String receiptId = message.headers.get("receipt");
        if(receiptId != null)
            headers.put("receipt-id", receiptId);
        String errorMessage ="The message:" + "\n----\n" + message.frameObjectToString() +"\n-----";
        connections.send(connectionId, new frameObject("ERROR", headers,errorMessage));
        connections.disconnect(connectionId);
        dataBase.disconnect(connectionId);
        shouldTerminate = true;
        System.out.println("error sent");
    }

    private void receipt(frameObject message){
        String receipt = message.headers.get("receipt");
        if(receipt != null){
            Map<String,String> headers = new HashMap<String,String>();
            headers.put("receipt-id", receipt);
            connections.send(connectionId, new frameObject("RECEIPT", headers, ""));
        }
    }
}