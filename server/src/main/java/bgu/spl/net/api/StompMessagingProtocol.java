package bgu.spl.net.api;

import bgu.spl.net.impl.stomp.ConnectionIMP;
import bgu.spl.net.impl.stomp.dataBase;


public interface StompMessagingProtocol<T> {
	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    void start(int connectionId, ConnectionIMP<T> connections, dataBase dataBase);
    
    void process(T message);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
