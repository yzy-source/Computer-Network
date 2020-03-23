package WebServer;

import java.io.IOException;  
import java.io.InputStream;  

//��ʵ�־�̬��Դ����������URI��Ӧ��mywebĿ¼�µ��ļ��У�������򿪣������ڷ���error
public class Request {
	private InputStream input; 
    private String myuri;     
    public Request(InputStream input) {  
        this.input = input;  
    }  
    //����request
    public void handle() { 
    	//����һ�������ַ�����һ��������buffer������������� ����СΪ2048���ֽ�
        StringBuffer request = new StringBuffer();  
        byte[] buffer = new byte[2048];  
        int i = 0;  
          
        try {  
        	//���������ж�ȡ�ֽڷ��ڻ�����buffer�У������ֽڸ���
            i = input.read(buffer); 
        } catch (IOException e) {  
            e.printStackTrace();  
            i = -1;  
        }  
          
        for(int k = 0; k < i; k++) {  
            request.append((char)buffer[k]);  
        }  
         //uri�ַ���
        myuri = handleUri(request.toString());     
        System.out.println("The request uri is :"+myuri);
    }  
      //�������󣬽���socket�ֽ�����������ȡuri��Ϣ
    private String handleUri(String requestData) {  
        int startindex, endindex;  
        //�Żء� �����ַ����е�һ�γ��ֵ�������û�з���-1��
        //������ ������ �ո� URI �ո� CRLF�س�
        startindex = requestData.indexOf(' ');  
        if(startindex != -1) {  
            endindex = requestData.indexOf(' ', startindex + 1);  
            if(endindex > startindex) {
            	//����ַ����Ӵ�;��startindex+1λ�õ�endindex
                return requestData.substring(startindex + 1, endindex);  
            }  
        }  
          
        return null;  
    }      
    //����uri�ַ���
    public String getUri() {  
        return myuri;  
    }  

}
