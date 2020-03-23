package WebServer;

import java.io.IOException;  
import java.io.InputStream;  

//仅实现静态资源服务器，将URI对应到myweb目录下的文件中，存在则打开，不存在返回error
public class Request {
	private InputStream input; 
    private String myuri;     
    public Request(InputStream input) {  
        this.input = input;  
    }  
    //处理request
    public void handle() { 
    	//定义一个请求字符串和一个缓冲区buffer用来存放输入流 ，大小为2048个字节
        StringBuffer request = new StringBuffer();  
        byte[] buffer = new byte[2048];  
        int i = 0;  
          
        try {  
        	//从输入流中读取字节放在缓冲区buffer中，返回字节个数
            i = input.read(buffer); 
        } catch (IOException e) {  
            e.printStackTrace();  
            i = -1;  
        }  
          
        for(int k = 0; k < i; k++) {  
            request.append((char)buffer[k]);  
        }  
         //uri字符串
        myuri = handleUri(request.toString());     
        System.out.println("The request uri is :"+myuri);
    }  
      //解析请求，接收socket字节流并处理，获取uri信息
    private String handleUri(String requestData) {  
        int startindex, endindex;  
        //放回‘ ’在字符串中第一次出现的索引，没有返回-1；
        //请求行 ：方法 空格 URI 空格 CRLF回车
        startindex = requestData.indexOf(' ');  
        if(startindex != -1) {  
            endindex = requestData.indexOf(' ', startindex + 1);  
            if(endindex > startindex) {
            	//输出字符串子串;从startindex+1位置到endindex
                return requestData.substring(startindex + 1, endindex);  
            }  
        }  
          
        return null;  
    }      
    //返回uri字符串
    public String getUri() {  
        return myuri;  
    }  

}
