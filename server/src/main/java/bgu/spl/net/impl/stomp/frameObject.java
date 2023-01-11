package bgu.spl.net.impl.stomp;


import java.util.HashMap;
import java.util.Map;

public class frameObject {
    String commandLine;
    public Map<String,String> headers = new HashMap<String,String>();
    String body;

    public frameObject(String commandLine, Map<String,String> map, String body){
        this.commandLine = commandLine;
        this.headers = map;
        this.body = body;
  
    }

    public String frameObjectToString(){
        String ans = "";
        if(commandLine != null)
            ans += commandLine +"\n";
        for(String field: headers.keySet()){
            if(field != null)
                ans = ans + field + ":" + headers.get(field) + "\n";
        }
    
        if(body != null)
            ans += "\n" + body;
        return ans;
    }


    static frameObject StringToframeObject(String str){
        if(str == ""){
            return null;
        }
        String[] lines = str.split("\n");
        String command = lines[0];
        Map<String,String> map = new HashMap<String,String>();
        String message = "";
        boolean inHeaders = true;
        for(int i = 1; i< lines.length;i++){
            String line = lines[i];
            if(inHeaders && line.isEmpty()){
                inHeaders = false;
            }
            else if(inHeaders){
                int dotIndexs = line.indexOf(':');
                String key;
                String value;
                if(dotIndexs != -1){
                    key = line.substring(0, dotIndexs);
                    value = line.substring(dotIndexs +1);
                }
                else{
                    //if client forgat to put ":"
                    key = line;
                    value = null;
                }
                map.put(key, value);
            }
            else{
                message = message + line + '\n';
            }
        }
        return new frameObject(command,map,message);
    }


}
