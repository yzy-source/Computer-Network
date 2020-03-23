package WebServer;

import java.io.File;  
import java.io.FileInputStream;  
import java.io.IOException;  
import java.io.OutputStream;  

public class Response {
	private OutputStream output;  
    private  Request request;  
    private static final int BUFFER_SIZE = 2048;  //��������С
    public Response(OutputStream output) {  
        this.output = output;  
    }  
    public void setRequest(Request request) {  
        this.request = request;  
    }  
      
    //����һ����̬��Դ(ֱ�Ӱ��ļ������ͻ���)���ͻ��ˣ������ط������ж�Ӧ���ļ��򷵻أ����򷵻�404ҳ��  
    public void Send() {  
        byte[] buffer = new byte[BUFFER_SIZE];  
        int filelength;  
        FileInputStream fis = null;  //����һ�����ڴ��ļ��ж�ȡ����
        try {  
        	//����ROOT·���µ�uri(�����ļ�)����һ����fileʵ��
            File file = new File(Server.ROOT, request.getUri());  
            if(file.exists()) {  
                fis = new FileInputStream(file); 
                //��inputstream�����ж�ȡָ���ֽڣ�����Ϊbuffer.length�������ݣ��ļ���β����-1
                //���ض�ȡ�ֽ���length
                filelength= fis.read(buffer);  
                while(filelength != -1) {  
                    //0 ��ʾ��������д�����ݵ�ƫ��
                    output.write(buffer, 0, filelength); 
                    //��ཫBUFFER_SIZE�����ݷ���buffer�����ض����ֽ�����0������ʼλ��
                    filelength = fis.read(buffer, 0, BUFFER_SIZE);     
                }  
            } else {  
                String errorMessage = "HTTP/1.1 404 File Not Found \r\n" +  
                        "Content-Type: text/html\r\n" +  
                        "Content-Length: 24\r\n" +  
                        "\r\n" +  
                        "<h1>File Not Found!</h1>";  
                output.write(errorMessage.getBytes());  
            }  
        } catch (Exception e) {  
            System.out.println(e.toString());  
        } finally {  
            if(fis != null) {  
                try {  
                    fis.close();  
                } catch (IOException e) {  
                    e.printStackTrace();  
                }  
            }  
        }  
    }  

}
