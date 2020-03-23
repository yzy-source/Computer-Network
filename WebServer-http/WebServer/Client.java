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

public class Client extends Thread{
	//����SHUTDOWN �ַ�������shutjudge �����ж��Ƿ��յ��ر�����
	private static final String SHUTDOWN = "/SHUTDOWN";
	private boolean shutjudge = false;
	public static int Threadnum=1; //�̺߳�
	
	//����һ��ʱ����󣬻�ȡ����ǰ��ʱ�䲢����ʱ����ʾ��ʽ
	Date date= new Date();
	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	String str = sdf.format(date);//����ǰʱ���ʽ��Ϊ��Ҫ������
	 
	public Socket socket;         //�ͻ����׽���
	public Client(Socket s){
		socket=s;
	}
	public void run(){ 
		    //�������������
            InputStream input = null;  
            OutputStream output = null; 
            try {  
            	input = socket.getInputStream();  
                output = socket.getOutputStream(); 
                System.out.println("\nMaking the thread "+(Threadnum++)); //����̺߳�
                //��÷�����ip��ַ�Ͷ˿ں�
                System.out.println("connect the client "+socket.getInetAddress()+" port is "+socket.getPort());                              
                System.out.println("accept the request! at "+str);//�������� 
                Request request = new Request(input);  
                request.handle();  
                System.out.println("request accepted! at"+str);
                //�ж��Ƿ��ǹر�����
                shutjudge = request.getUri().equals(SHUTDOWN); 
                if(!shutjudge)
                {
                //�������󲢷��ؽ�� 
                System.out.println("WebServer is handling the request");
                Response response = new Response(output);  
                response.setRequest(request);  
                response.Send();  
                //�ر�socket  
                System.out.println("close socket\n");
                socket.close();
                } 
                else{ 
                	//����������Ϊ�رգ���ر�Socket ��ServerSocket  
                	System.out.println("The Web Server has received a shutdown command!");
                    Response response = new Response(output);  
                    response.setRequest(request);  
                    response.Send();  
                    System.out.println("Close the Client and WebServer");
                    socket.close();
                    System.exit(1);
                }
                
            } catch (IOException e) {  
                e.printStackTrace();   
            } 
		}
}

