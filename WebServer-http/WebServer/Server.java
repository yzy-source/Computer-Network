package WebServer;

import java.io.File;  
import java.io.IOException;  
import java.io.InputStream;  
import java.io.OutputStream;  
import java.net.InetAddress;  
import java.net.ServerSocket;  
import java.net.Socket;  
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;

//Server Model 
public class Server {
	public static final String ROOT = System.getProperty("user.dir") + File.separator + "myweb"; 
    public static void main(String[] args) { 
    	
       //创建一个时间对象，获取到当前的时间并设置时间显示格式
    	Date date= new Date();
    	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    	String str = sdf.format(date);
    	
        ServerSocket serverSocket  = null;  
        int  port = 8001;  //端口号8001
        try {  
        	//创建一个服务器一直工作，服务器连接到本地主机（127.0.0.1）
        	//1代表服务器套接字开始拒绝传入的请求之前，传入的连接请求的最大队列长度
            serverSocket = new ServerSocket(port, 1, InetAddress.getByName("127.0.0.1"));  
            System.out.println("WebServer started!! at "+str);
                      
            while(true){
            	//Web服务器一直进行监听，等待客户端请求
            	Socket socket = null;  
                socket = serverSocket.accept(); 
                System.out.println("WebServer is listening and Waiting for the Client Request…… ");
            
                Client myclient=new Client(socket);
            	myclient.start();
            }
            
        } catch (UnknownHostException e) {  
            e.printStackTrace();  
            System.exit(-1);  
        } catch (IOException e) {  
            e.printStackTrace();  
            System.exit(-1);  
        } 
    }    
}
