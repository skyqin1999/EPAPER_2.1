
#include "io430.h"
#define epdcklow           P2OUT &= 0xf7
#define epdckhigh          P2OUT |= 0x08
#define epddalow           P2OUT &= 0xef
#define epddahigh          P2OUT |= 0x10
#define epdcslow           P3OUT &= 0xef
#define epdcshigh          P3OUT |= 0x10
#define epddclow           P3OUT &= 0xdf
#define epddchigh          P3OUT |= 0x20
#define epdrstlow          P3OUT &= 0xbf
#define epdrsthigh         P3OUT |= 0x40
#define epdbslow          P3OUT &= 0xfd
#define epdunknowhigh          P3OUT &= 0x80
#define epdon           P2OUT &= 0xbf
#define epdoff          P2OUT |= 0x40

#define rfcklow            P1OUT &= 0xef
#define rfckhigh           P1OUT |= 0x10
#define rfdalow            P1OUT &= 0xfb
#define rfdahigh           P1OUT |= 0x04
#define rfcslow            P1OUT &= 0xf7
#define rfcshigh           P1OUT |= 0x08
#define rfon               P2OUT &= 0x7f
#define rfoff              P2OUT |= 0x80

#define spiromcshigh       P3OUT |= 0x01
#define spiromcslow        P3OUT &= 0xfe
void InitClk(void);
void InitGpio(void);
void InitEpd(void);
void sendbyte(int sdbyte);
void epd_send_cmd(int cmd);
void epd_sendcmddata(int cmd,int data);
void Delay(unsigned int nCount);
void Delaylong(unsigned int n10Count);
extern const unsigned char img1[];
extern const unsigned char img2[];
extern const unsigned char epdinit[];
unsigned int s;
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  InitGpio();
  InitClk();
  InitEpd(); //about 5s
  
  epd_sendcmddata(0x4e,0x0);  
  epd_sendcmddata(0x4f,0x0);    
  epd_send_cmd(0x24);            //  write ram     
   epdcshigh;
   for(s=0;s<4000;s++)
      { 
        epdcslow;    
        sendbyte(img1[s]);
        epdcshigh;
      }
    epd_sendcmddata(0x22,0xc7); 
    epd_send_cmd(0x20);           
  
    while(1);
}
void InitEpd(void)
{  
        unsigned char i;
        epd_send_cmd(0x12);
        Delaylong(1);
        for ( i = 0; i <= 0x33; i++ )
        {
                if ( (i == 0x0) || (i == 0x3) || (i == 0x5) || (i == 0x7) || 
                    (i == 0x9) || (i == 0xc) || (i == 0xf) || (i == 0x11) ||
                    (i == 0x13) ||  (i == 0x32)) 
                {
                  epddclow;
                }
              epdcslow;
              sendbyte(epdinit[i]);
              epdcshigh;
              epddchigh;
              asm("nop");
              asm("nop");
         }
  
}


void InitGpio(void)
{
  rfcslow;
  rfcklow;
  rfdalow;
  spiromcslow;
  epdunknowhigh;
  epdbslow ;  
  epdon;
  rfoff;
  P1DIR = P2 | P3 | P4 | P6 | P7 ;
  P2DIR = P3 | P4 | P5 | P6 | P7 ;
  P3DIR = P0 | P1 | P4 | P5 | P6 | P7 ;
  P1SEL = 0X0;
  P1SEL2 = 0X0;
  P2SEL = 0X0;
  P2SEL2 = 0X0;
  P3SEL = 0X0;
  P3SEL2 = 0X0;   
  Delaylong(3);
  epdrsthigh;
  Delaylong(3);
  epdrstlow;
  Delaylong(3);
  epdrsthigh;  
  
}

void InitClk(void)
{
  DCOCTL = 0x0000;
  BCSCTL1 = CALBC1_8MHZ;                    // Set range
  DCOCTL = CALDCO_8MHZ;

  BCSCTL3 |= LFXT1S_2;                   /* Mode 2 for LFXT1 : VLO */
  IFG1 &= !(OFIFG);
  Delay(512);
  BCSCTL1 |= DIVA_3 ;                     /* ACLK Divider 3: /8 */  
}

void epd_sendcmddata(int cmd,int data)
{
  epddclow;
  epdcslow;
  sendbyte(cmd);
  epddchigh;
  sendbyte(data);
  epdcshigh;
 }
void epd_send_cmd(int cmd)
{
  epddclow;
  epdcslow;
  sendbyte(cmd);
  epdcshigh;
  epddchigh;
}


void sendbyte(int sdbyte)
{
  volatile unsigned int i;
  for (i =0; i<8;i++)
  {
     epdcklow; 
    if (sdbyte & 0x80)
      { epddahigh;
        }
    else
      { epddalow;
       }
    sdbyte <<=1;
     epdckhigh;  
  }
  epdcklow;
} 

void Delay(unsigned int nCount)
{
  for(;nCount!=0;nCount--)
  {
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  }
  
} 

void Delaylong(unsigned int n10Count)
{
        unsigned char i;
            while(n10Count--)
            {
              for(i = 0; i < 10; i ++)
                {
                        Delay(10000);
                }
            }
}