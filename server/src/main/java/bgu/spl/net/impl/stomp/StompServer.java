package bgu.spl.net.impl.stomp;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {


        Integer port = Integer.parseInt(args[0]);
        String host = args[1];

        if(host.equals("tpc")){
            Server.threadPerClient(
                port, //port
                () -> new StompProtocolIMP(), //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
        else if(host.equals("reactor")){
            Server.reactor(
                 Runtime.getRuntime().availableProcessors(),
                 port, //port
                 () ->  new StompProtocolIMP(), //protocol factory
                 StompMessageEncoderDecoder::new //message encoder decoder factory
         ).serve();
        }
        else{
            //EXCPTION
        }
    }
}
