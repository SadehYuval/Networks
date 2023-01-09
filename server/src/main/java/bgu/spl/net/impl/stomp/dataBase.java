package bgu.spl.net.impl.stomp;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;

import java.util.Iterator;


public class dataBase {

        //***dataBase***
        //userName -> password
        public ConcurrentHashMap<String,String> usernamePassword = new ConcurrentHashMap<String,String>();
        //ueserName -> connectionId
        public ConcurrentHashMap<String,Integer> usernameConnectionId = new ConcurrentHashMap<String,Integer>();
        //connectionId -> {userName}
        public ConcurrentHashMap<Integer,ConcurrentSkipListSet<String>> connectionIdUserNameMap = new ConcurrentHashMap<Integer,ConcurrentSkipListSet<String>>();
        //cannel -> {<connectionId -> subscriptionId>}
        public ConcurrentHashMap<String,ConcurrentHashMap<Integer,String>>  channelsMap = new ConcurrentHashMap<String,ConcurrentHashMap<Integer,String>>(); 
        //connectionId -> {<subscriptionId -> cannel>}
        public ConcurrentHashMap<Integer,ConcurrentHashMap<String,String>> connectionIdMap = new ConcurrentHashMap<Integer,ConcurrentHashMap<String,String>>(); 
        //message-Id
        protected AtomicInteger massegeId = new AtomicInteger(0);




        public void disconnect(int connectionId){
            //connectionIdMap = connectionId -> {<subscriptionId -> cannel>}
            //channelsMap =- cannel -> {<connectionId -> subscriptionId>}
            //usernameConnectionId = ueserName -> connectionId
            //connectionIdUserNameMap = connectionId -> {userName}
            ConcurrentHashMap<String,String> subIdToChannel = connectionIdMap.get(connectionId);
            if(subIdToChannel != null){
                for(Map.Entry<String,String> iter: subIdToChannel.entrySet()){
                    ConcurrentHashMap<Integer,String> cIdToSunId = channelsMap.get(iter.getValue());
                    if(cIdToSunId.get(connectionId) != null)
                        cIdToSunId.remove(connectionId);
                }
                connectionIdMap.remove(connectionId);
            }    
            ConcurrentSkipListSet<String> userNames = connectionIdUserNameMap.get(connectionId);
            if(userNames != null){
                Iterator<String> iter = userNames.iterator();
                while(iter.hasNext()){
                    String usrName = iter.next();
                    usernameConnectionId.remove(usrName);

                }
                connectionIdUserNameMap.remove(connectionId);
            } 
        }


        public Integer getMessageId(){
            int i;
            do{
                i = massegeId.get();
            }while(!massegeId.compareAndSet(i, i+1));
            return i;
        }

        public boolean managedToConnect(String userName,String password,Integer connectionId){
            return(newUser(userName,password,connectionId) || oldUser(userName,password,connectionId));
        }
    
    
        private boolean newUser(String userName,String password,Integer connectionId){
           String correctPassword = usernamePassword.get(userName);
           if(correctPassword == null){
                usernamePassword.put(userName, password);
                usernameConnectionId.put(userName, connectionId);
                ConcurrentSkipListSet<String> useNames =  connectionIdUserNameMap.get(connectionId);
                if(useNames == null){
                    useNames = new ConcurrentSkipListSet<>();  
                    connectionIdUserNameMap.put(connectionId, useNames); 
                }
                connectionIdUserNameMap.get(connectionId).add(userName);                
                return true;
           }
           return false;
        }
    
        private boolean oldUser(String userName,String password,Integer connectionId){
            Integer currectConnectionId = usernameConnectionId.get(userName);
            ConcurrentSkipListSet<String> useNames =  connectionIdUserNameMap.get(connectionId);
            if(usernamePassword.get(userName).equals(password)){//userName - pasword match
                if(currectConnectionId == null){//user not login from another client
                    usernameConnectionId.put(userName, connectionId);
                    if(useNames == null){
                        useNames = new ConcurrentSkipListSet<>();  
                        connectionIdUserNameMap.put(connectionId, useNames); 
                    }
                    connectionIdUserNameMap.get(connectionId).add(userName);    
                    return true;
                }
                else//user already login from another client
                    return false;    
            }
           return false;
        }
    
    
    
    
    
        public boolean subscribe(int connectionId,String subId, String channel){
            //cannel -> {<connectionId -> subscriptionId>}
            //currentChannelMap = Map:connectionId - subscriptionId
            ConcurrentHashMap<Integer,String> currentChannelMap =  channelsMap.get(channel);
            //connectionId -> {<subscriptionId -> channel>}
            //subIdToChannel = Map:subscriptionId - cannel
            ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
            if(currentChannelMap == null){
                currentChannelMap = new ConcurrentHashMap<Integer,String>();
                channelsMap.put(channel, currentChannelMap);
            }
            if(subIdToChannel == null){
                subIdToChannel = new ConcurrentHashMap<>();
                connectionIdMap.put(connectionId, subIdToChannel);
            }
            String realSubscriptionId = currentChannelMap.get(connectionId);
            String realChannel = subIdToChannel.get(subId);
            if(realSubscriptionId != null || realChannel != null){
                return false;
            }
            else{
                currentChannelMap.put(connectionId, subId);
                subIdToChannel.put(subId, channel);
                return true;
            }
        }
    
    
    
        public boolean unsubscribe(String subId, int connectionId){
            //connectionIdMap = Map:connectionId -> {<subscriptionId -> channel>}
            //subIdToChannel = Map:subscriptionId - cannel
            ConcurrentHashMap<String,String> subIdToChannel =  connectionIdMap.get(connectionId);
            if(subIdToChannel == null){
                return false;
            }
            else{
                String realChannel = subIdToChannel.get(subId);
                if(realChannel == null){
                    return false;
                }
                else{
                    //cannel -> {<connectionId -> subscriptionId>}
                    //currentChannelMap = Map:connectionId - subscriptionId
                    ConcurrentHashMap<Integer,String> currentChannelMap =  channelsMap.get(realChannel);
                    if(currentChannelMap == null)
                        return false;
                    String realSubscriptionId = currentChannelMap.get(connectionId);
                    if(realSubscriptionId == null)
                        return false;
                    subIdToChannel.remove(subId);
                    currentChannelMap.remove(connectionId);
                    return true;                                       
    
                }
            }
        }
    
}
