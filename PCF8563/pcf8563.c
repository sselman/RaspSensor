    #include <bcm2835.h>  
    #include <stdio.h>
    #include <unistd.h>
    unsigned char WriteBuf[];  
    unsigned char ReadBuf;  
#define changeHexToInt(hex)		( ( ((hex)>>4) *10 ) + ((hex)%16) )  
#define MIN    0x02  
#define SEC    0x03 
#define HOUR   0x04 
#define DAY    0x05 
#define WEEK   0x06 
#define MONTH  0x07 
#define YEAR   0x08 
unsigned char g8563_Store[4]; 
unsigned char  c8563_Store[4]={0x04,0x59,0x10,0x01};

void P8563_settime() 
{ 
 	       WriteBuf[0] = 2; 
 	       WriteBuf[1] = g8563_Store[0]; 
         bcm2835_i2c_write(&WriteBuf,2); 

 	       WriteBuf[0] = 3; 
 	       WriteBuf[1] = g8563_Store[1]; 
         bcm2835_i2c_write(&WriteBuf,2); 
      
 	       WriteBuf[0] = 4; 
 	       WriteBuf[1] = g8563_Store[2]; 
         bcm2835_i2c_write(&WriteBuf,2); 
  	       
 	       WriteBuf[0] = 6; 
 	       WriteBuf[1] = g8563_Store[3]; 
         bcm2835_i2c_write(&WriteBuf,2); 
}

void P8563_init() 
{ 
    unsigned char i; 
 //   WriteBuf[0] = 0x0a;  
 //   bcm2835_i2c_write_read_rs(&WriteBuf[0] ,1, &ReadBuf,1);
 
 //   if((ReadBuf)!=0x8)
 //   { 
        for(i=0;i<=3;i++)  
        g8563_Store[i]=c8563_Store[i]; 
        P8563_settime(); 
        printf("P8563_settime..........\n");  
         
 	       WriteBuf[0] = 0x0; 
 	       WriteBuf[1] = 0x00; 
         bcm2835_i2c_write(&WriteBuf,2); 

 	       WriteBuf[0] = 0xa; 
 	       WriteBuf[1] = 0x8; 
         bcm2835_i2c_write(&WriteBuf,2); 
        
 	       WriteBuf[0] = 0x01; 
 	       WriteBuf[1] = 0x12; 
         bcm2835_i2c_write(&WriteBuf,2); 
         
 	       WriteBuf[0] = 0xd; 
 	       WriteBuf[1] = 0xf0; 
         bcm2835_i2c_write(&WriteBuf,2);       
         
//    } 
} 
void P8563_Readtime() 
{   unsigned char time[7];   
	 	     WriteBuf[0] = MIN;  
         bcm2835_i2c_write_read_rs( &WriteBuf[0] ,1, &time,7);  
         
    g8563_Store[0]=time[0]&0x7f; 
    g8563_Store[1]=time[1]&0x7f; 
    g8563_Store[2]=time[2]&0x3f; 
    g8563_Store[3]=time[4]&0x07; 
      
    g8563_Store[0]=changeHexToInt(g8563_Store[0]);
	g8563_Store[1]=changeHexToInt(g8563_Store[1]);
    g8563_Store[2]=changeHexToInt(g8563_Store[2]);
	g8563_Store[3]=changeHexToInt(g8563_Store[3]);
} 
    int main(int argc, char **argv)  
    {  
        if (!bcm2835_init())  
        return 1;  
        bcm2835_i2c_begin();  
        bcm2835_i2c_setSlaveAddress(0x51);  
        bcm2835_i2c_set_baudrate(10000);  
        printf("start..........\n"); 
    P8563_init() ;
      
        while(1)  
        {  
        
         P8563_Readtime();

	       printf("Hour:%d%d Mins:%d Secs:%d\n",g8563_Store[2]/10,g8563_Store[2]%10,g8563_Store[1],g8563_Store[0]);

            bcm2835_delay(1000);  


        }  
         
        bcm2835_i2c_end();  
        bcm2835_close();  
      
        return 0;  
    }  
