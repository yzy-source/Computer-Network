package WebServer;

import java.io.File;  
import java.io.FileInputStream;  
import java.io.IOException;  
import java.io.OutputStream;  

public class Response {
	private OutputStream output;  
    private  Request request;  
    private static final int BUFFER_SIZE = 2048;  //缓冲区大小
    public Response(OutputStream output) {  
        this.output = output;  
    }  
    public void setRequest(Request request) {  
        this.request = request;  
    }  
      
    //发送一个静态资源(直接把文件发给客户端)给客户端，若本地服务器有对应的文件则返回，否则返回404页面  
    public void Send() {  
        byte[] buffer = new byte[BUFFER_SIZE];  
        int filelength;  
        FileInputStream fis = null;  //此流一般用于从文件中读取数据
        try {  
        	//根据ROOT路径下的uri(请求文件)创建一个新file实例
            File file = new File(Server.ROOT, request.getUri());  
            if(file.exists()) {  
                fis = new FileInputStream(file); 
                //从inputstream对象中读取指定字节（长度为buffer.length）的数据，文件结尾返回-1
                //返回读取字节数length
                filelength= fis.read(buffer);  
                while(filelength != -1) {  
                    //0 表示在数组中写入数据的偏移
                    output.write(buffer, 0, filelength); 
                    //最多将BUFFER_SIZE个数据放入buffer，返回读入字节数，0代表起始位置
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
