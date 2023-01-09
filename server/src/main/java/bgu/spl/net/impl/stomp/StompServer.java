package bgu.spl.net.impl.stomp;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {

       args = new String[]{"t","TPC","r"};

        

        if(args[1] == "TPC"){
            Server.threadPerClient(
                7777, //port
                () -> new StompProtocolIMP(), //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
        else if(args[1] == "REACTOR"){
            Server.reactor(
                 Runtime.getRuntime().availableProcessors(),
                 7777, //port
                 () ->  new StompProtocolIMP(), //protocol factory
                 StompMessageEncoderDecoder::new //message encoder decoder factory
         ).serve();
        }
        else{
            //EXCPTION
        }
    }
}
