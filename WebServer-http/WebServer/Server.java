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
    	
       //����һ��ʱ����󣬻�ȡ����ǰ��ʱ�䲢����ʱ����ʾ��ʽ
    	Date date= new Date();
    	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    	String str = sdf.format(date);
    	
        ServerSocket serverSocket  = null;  
        int  port = 8001;  //�˿ں�8001
        try {  
        	//����һ��������һֱ���������������ӵ�����������127.0.0.1��
        	//1����������׽��ֿ�ʼ�ܾ����������֮ǰ���������������������г���
            serverSocket = new ServerSocket(port, 1, InetAddress.getByName("127.0.0.1"));  
            System.out.println("WebServer started!! at "+str);
                      
            while(true){
            	//Web������һֱ���м������ȴ��ͻ�������
            	Socket socket = null;  
                socket = serverSocket.accept(); 
                System.out.println("WebServer is listening and Waiting for the Client Request���� ");
            
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
