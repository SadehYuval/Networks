package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class StompConnections implements Connections<frameObject>{

    @Override
    public boolean send(int connectionId, frameObject msg) {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void send(String channel, frameObject msg) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void disconnect(int connectionId) {
        // TODO Auto-generated method stub
        
    }
}


// //***connecting***
// //userName -> password
// public ConcurrentHashMap<String,String> usernamePassword = new ConcurrentHashMap<String,String>();
// //ueserName -> connectionId
// public ConcurrentHashMap<String,Integer> usernameConnectionId = new ConcurrentHashMap<String,Integer>();
// //CH
// public ConcurrentSkipListSet<ConnectionHandler<frameObject>> CH = new ConcurrentSkipListSet<ConnectionHandler<frameObject>>();
// //
// //***public boolean send(int connectionId, T msg)***
// //connectionId -> CH
// public ConcurrentHashMap<Integer,ConnectionHandler<frameObject>> connectionIdConnectionHandler = new ConcurrentHashMap<Integer,ConnectionHandler<frameObject>>();
// //***public void send(String channel, T msg)***
// //cannel -> {<connectionId -> subscriptionId>}
// public ConcurrentHashMap<String,ConcurrentHashMap<Integer,String>>  channelsMap = new ConcurrentHashMap<String,ConcurrentHashMap<Integer,String>>(); 
// //connectionId -> {<subscriptionId -> cannel>}
// public ConcurrentHashMap<Integer,ConcurrentHashMap<String,String>> connectionIdMap = new ConcurrentHashMap<Integer,ConcurrentHashMap<String,String>>(); 
// //
// protected AtomicInteger massegeId = new AtomicInteger(0);


// public void Connections(){

// }

//     @Override
//     public boolean send(int connectionId, frameObject msg){
//         ConnectionHandler<frameObject> Ch = connectionIdConnectionHandler.get(connectionId);
//         if(Ch != null){
//             Ch.send(msg);
//             return true;
//         }
//         return false;    
//     }

//     @Override
//     public void send(String channel, frameObject msg){
//         ConcurrentHashMap<Integer,String> connectionIdSubscriptionId = channelsMap.get(channel);
//         for(Map.Entry<Integer,String> entry: connectionIdSubscriptionId.entrySet()){
//             Map<String,String> headers = new HashMap<String,String>();
//             for(Map.Entry<String,String> msgHeader: msg.headers.entrySet() ){
//                 headers.put(msgHeader.getKey(), msgHeader.getValue());
//             }
//             headers.put("subscription", entry.getValue());
//             Integer msgId = getMessageId();
//             headers.put("message-id", msgId.toString());
//             send(entry.getKey(), new frameObject("MESSAGE",headers, msg.body));
//         }
//     }

//     @Override
//     public void disconnect(int connectionId){
//         //connectionIdConnectionHandler = connectionId -> CH  
//         //CH = CH
//         //connectionIdMap = connectionId -> {<subscriptionId -> cannel>}
//         //channelsMap =- cannel -> {<connectionId -> subscriptionId>}
//         //usernameConnectionId = ueserName -> connectionId
//         ConnectionHandler<frameObject> ch = connectionIdConnectionHandler.get(connectionId);

//         connectionIdConnectionHandler.remove(connectionId);
//         CH.remove(ch);
//         String userName = usernameConnectionId.
//         //usernameConnectionId.remove(connectionId);
//         ConcurrentHashMap<String,String>> subToChannelMap = connectionIdMap.get(connectionId);
//         for(Map.Entry<String,String> getChannel: subToChannelMap.entrySet() ){
//             String channel = getChannel.getValue();
//             (channelsMap.get(channel)).remove(connectionId);
//         }
//         connectionIdMap.remove(connectionId);      
//     }

//     //geters
//     //public int getConnectionId(ConnectionHandler<T> connectionHandler){
//       //  return CH_userName.get(connectionHandler);
//     //}
//     public boolean managedToConnect(String userName,String password,Integer connectionId){
//         return(newUser(userName,password,connectionId) || oldUser(userName,password,connectionId));
//     }


//     private boolean newUser(String userName,String password,Integer connectionId){
//        String correctPassword = usernamePassword.get(userName);
//        if(correctPassword == null){
//             usernamePassword.put(userName, password);
//             usernameConnectionId.put(userName, connectionId);
            
//             return true;
//        }
//        return false;
//     }

//     private boolean oldUser(String userName,String password,Integer connectionId){
//         Integer currectConnectionId = usernameConnectionId.get(userName);
//         if(usernamePassword.get(userName).equals(password)){
//             if(currectConnectionId == null){
//                 usernameConnectionId.put(userName, connectionId);
//                 return true;
//             }
//             else
//                 return false;    
//         }
//        return false;
//     }



//     public Integer getMessageId(){
//         int i;
//         do{
//             i = massegeId.get();
//         }while(!massegeId.compareAndSet(i, i+1));
//         return i;
//     }

//     public boolean subscribe(int connectionId,String subId, String channel){
//         //cannel -> {<connectionId -> subscriptionId>}
//         //currentChannelMap = Map:connectionId - subscriptionId
//         ConcurrentHashMap<Integer,String> currentChannelMap =  channelsMap.get(channel);
//         //connectionId -> {<subscriptionId -> channel>}
//         //subIdToChannel = Map:subscriptionId - cannel
//         ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
//         if(currentChannelMap == null){
//             currentChannelMap = new ConcurrentHashMap<Integer,String>();
//             channelsMap.put(channel, currentChannelMap);
//         }
//         if(subIdToChannel == null){
//             subIdToChannel = new ConcurrentHashMap<>();
//             connectionIdMap.put(connectionId, subIdToChannel);
//         }
//         String realSubscriptionId = currentChannelMap.get(connectionId);
//         String realChannel = subIdToChannel.get(subId);
//         if(realSubscriptionId != null || realChannel != null){
//             return false;
//         }
//         else{
//             currentChannelMap.put(connectionId, subId);
//             subIdToChannel.put(subId, channel);
//             return true;
//         }
//     }



//     public boolean unsubscribe(String subId, int connectionId){
//         //connectionId -> {<subscriptionId -> channel>}
//         //subIdToChannel = Map:subscriptionId - cannel
//         ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
//         if(subIdToChannel == null){
//             return false;
//         }
//         else{
//             String realChannel = subIdToChannel.get(subId);
//             if(realChannel == null){
//                 return false;
//             }
//             else{
//                 //cannel -> {<connectionId -> subscriptionId>}
//                 //currentChannelMap = Map:connectionId - subscriptionId
//                 ConcurrentHashMap<Integer,String> currentChannelMap =  channelsMap.get(realChannel);
//                 if(currentChannelMap == null)
//                     return false;
//                 String realSubscriptionId = currentChannelMap.get(connectionId);
//                 if(realSubscriptionId == null)
//                     return false;
//                 subIdToChannel.remove(subId);
//                 currentChannelMap.remove(connectionId);
//                 return true;                                       

//             }
//         }
//     }


// }


//     /*public boolean subscribe(int connectionId,String subId, String channel){
//         ConcurrentHashMap<Integer,String> currentChannelMap =  channelsMap.get(channel);
//         if(currentChannelMap == null){
//             //channel doesnt exist
//             //cannel -> {<connectionId -> subscriptionId>}
//             currentChannelMap = new ConcurrentHashMap<Integer,String>();
//             currentChannelMap.put(connectionId, subId);
//             channelsMap.put(channel, currentChannelMap);
//             //connectionId -> {<subscriptionId -> cannel>}
//             //connectionIdMap
//             ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
//             if(subIdToChannel ==null){
//                 subIdToChannel = new ConcurrentHashMap<>();
//                 subIdToChannel.put(subId, channel);
//                 connectionIdMap.put(connectionId, subIdToChannel);
//             }
//             else{
//                 subIdToChannel.put(subId, channel);
//             }
//             return true;
//         }
//         else{
//             //channel exist
//             String subscriptionId = currentChannelMap.get(connectionId);
//             if(subscriptionId == null){
//                 //client dosnt register to channel
//                 currentChannelMap.put(connectionId , subId);
//                 ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
//                 if(subIdToChannel == null){
//                     //subIdToChannel Map fo client dosent exist
//                     subIdToChannel = new ConcurrentHashMap<>();
//                     subIdToChannel.put(subId, channel);
//                     connectionIdMap.put(connectionId, subIdToChannel);
//                 }
//                 else{
//                     //subIdToChannel Map fo client already exist
//                     subIdToChannel.put(subId, channel);
//                 }
//                 return true;
//             }
//             else{
//                 //client register to channel
//                 return false;

//             }
//         }
//     }*/
