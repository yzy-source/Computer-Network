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
	//定义SHUTDOWN 字符串，和shutjudge 用来判断是否收到关闭命令
	private static final String SHUTDOWN = "/SHUTDOWN";
	private boolean shutjudge = false;
	public static int Threadnum=1; //线程号
	
	//创建一个时间对象，获取到当前的时间并设置时间显示格式
	Date date= new Date();
	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	String str = sdf.format(date);//将当前时间格式化为需要的类型
	 
	public Socket socket;         //客户端套接字
	public Client(Socket s){
		socket=s;
	}
	public void run(){ 
		    //定义输入输出流
            InputStream input = null;  
            OutputStream output = null; 
            try {  
            	input = socket.getInputStream();  
                output = socket.getOutputStream(); 
                System.out.println("\nMaking the thread "+(Threadnum++)); //输出线程号
                //获得服务器ip地址和端口号
                System.out.println("connect the client "+socket.getInetAddress()+" port is "+socket.getPort());                              
                System.out.println("accept the request! at "+str);//接收请求 
                Request request = new Request(input);  
                request.handle();  
                System.out.println("request accepted! at"+str);
                //判断是否是关闭命令
                shutjudge = request.getUri().equals(SHUTDOWN); 
                if(!shutjudge)
                {
                //处理请求并返回结果 
                System.out.println("WebServer is handling the request");
                Response response = new Response(output);  
                response.setRequest(request);  
                response.Send();  
                //关闭socket  
                System.out.println("close socket\n");
                socket.close();
                } 
                else{ 
                	//若请求命令为关闭，则关闭Socket 和ServerSocket  
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

