package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;

import org.omg.Messaging.SyncScopeHelper;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ConnectionIMP<T> implements Connections<T> {

    //connectionId -> CH
    public ConcurrentHashMap<Integer,ConnectionHandler<T>> connectionIdConnectionHandler = new ConcurrentHashMap<Integer,ConnectionHandler<T>>();

    @Override
    public boolean send(int connectionId, T msg) {
        System.out.println("MESSAGE SENT TO CLIENT:\n" + ((frameObject)msg).frameObjectToString());
        ConnectionHandler<T> Ch = connectionIdConnectionHandler.get(connectionId);
        if(Ch != null){
            Ch.send(msg);
            return true;
        }
        return false; 
        
    }
    
    @Override
    public void send(String channel, T msg) {
    }

    @Override
    public void disconnect(int connectionId) {
        connectionIdConnectionHandler.remove(connectionId);
        
    }

    
    
}
