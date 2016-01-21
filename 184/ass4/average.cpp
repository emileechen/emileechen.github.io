// set up for 12 bits 


#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;

double PQ10000_f( double V);
double PQ10000_r( double L);
float  BBC_f(float V, float sf=1.2);
float  BBC_r(float L, float sr=1.0);
float  BT1886_f(float V, float s=1.2);
float  BT1886_r(float L, float s=1.2);

bool FULL=false;
bool R2020=true;




main(int argc, char* argv[])
{

	TIFF* tifIn;
	

	     
	     //Process Args:
	     short arg = 1;
	     while(arg < argc) {

     
	     	
 

		     if(strcmp(argv[arg],"-i")==0) {
					arg++;
					if(arg < argc)tifIn=TIFFOpen(argv[arg], "r");	     	
		     } 

		     if(strcmp(argv[arg],"-FULL")==0) {
		     	 FULL=true;
		     }	
		     
		     if(strcmp(argv[arg],"-LEGAL")==0) {
		     	 FULL=false;
		     }		     	          

		     if(strcmp(argv[arg],"-2020")==0) {
		     	 R2020=true;
		     }	
		     
		     if(strcmp(argv[arg],"-P3")==0) {
		     	 R2020=false;
		     }		     		     
		     if(strcmp(argv[arg],"-h")==0) {
		     	 printf("\n ARGS:\n -i <infile> ");
		     	 exit(0);
		     }
	     

	     arg++;
	     }
	     
	     

	

    if (tifIn) {
        tdata_t buf1;

        tstrip_t strip;
        uint32* bc;
        uint32 stripsize;
        uint32 pixel;
        uint32 pixelStart = 0;
        uint32 ib;
        int    numStrips;

        
        numStrips = TIFFNumberOfStrips(tifIn);

       
      
        //get count of bytes in each strip of image as vector of rows/lines
        TIFFGetField(tifIn, TIFFTAG_STRIPBYTECOUNTS, &bc);
        stripsize = bc[0];
        
        buf1 = _TIFFmalloc(stripsize);
        
        //printf("numStrips = %d,  stripsize = %d\n",numStrips, stripsize);

        // pixelstart will be byte location of first pixel
        // stripsize will be byte location just after last pixel 
        pixelStart = 0; // assume use entire images and both are same size
        
        uint32 R;
        uint32 G;
        uint32 B;
        uint32 A;

        unsigned short** R1;
        unsigned short** G1; 
        unsigned short** B1;	
        unsigned short *Line; // output line         

        int arraySizeX = stripsize/6 - pixelStart/6; // eg 3840
        int arraySizeY = numStrips;

	     R1 = (unsigned short**) malloc(arraySizeX*sizeof(unsigned short*)); // cols
        for (int i = 0; i < arraySizeX; i++)
            R1[i] = (unsigned short*) malloc(arraySizeY*sizeof(unsigned short)); //rows


	     G1 = (unsigned short**) malloc(arraySizeX*sizeof(unsigned short*)); // cols
        for (int i = 0; i < arraySizeX; i++)
            G1[i] = (unsigned short*) malloc(arraySizeY*sizeof(unsigned short)); // rows
            
	     B1 = (unsigned short**) malloc(arraySizeX*sizeof(unsigned short*)); // cols
        for (int i = 0; i < arraySizeX; i++)
            B1[i] = (unsigned short*) malloc(arraySizeY*sizeof(unsigned short)); // rows
       

 
           


        // read lines to cover arraySizeY number of lines (e.g. 2160)
        for (strip = 0; strip < numStrips; strip++) {
        	
            TIFFReadRawStrip(tifIn, strip, buf1, bc[0]);

                  
            	
            // chunk scan line into 3 RGB 16bit shorts
            // so skip 16 values per block or 32 bytes
            // 32768 bytes is 4096 * 4(rgba) * 16bits = 4096 * 4 * 2
            for (pixel = pixelStart; pixel < stripsize; pixel+=6) {

				  // Image #1 16 bit values so 2 bytes per index
              R = static_cast<unsigned short *>(buf1)[pixel/2];
              G = static_cast<unsigned short *>(buf1)[pixel/2+1];
              B = static_cast<unsigned short *>(buf1)[pixel/2+2];

              R1[pixel/6][strip] =  R;
              G1[pixel/6][strip] =  G;
              B1[pixel/6][strip] =  B;       
              

              
                                          
            }   // end of scanline blocking
        } // end of loop of scanlines
        
        _TIFFfree(buf1);
        TIFFClose(tifIn);
        
	    double mean = 0;
	    double median = 0;
	    double maxFALL = 0;
	    double maxCLL = 0;
	    float L = 0.0;
	    float LMAX = 0.0;
	    float black = 4096.0;
		float Range = 60160.0 -black;
		
		if(FULL) {
			black = 0.0;
			Range = 65535.0;
		}		
		
		float scale = 65535.0/Range;
		

	    
// 2020: (0.2627*(float)R + 0.6780*(float)G + 0.0593*(float)B) +0.5;
//  709: (0.2126*(float)R + 0.7152*(float)G + 0.0722*(float)B) +0.5;

/*  P3D65 to XYZ
{ { 0.486571,	0.228975,	0,	0},
  { 0.265668,	0.691739,	0.0451134,	0},
  { 0.198217,	0.0792869,	1.04394,	0},
  { 0,	0,	0,	1} }
*/	
		    
		
			for (int line = 0;line < arraySizeY;line++)
			{
				
				for (unsigned int pixel = 0; pixel < 3*arraySizeX;pixel+=3)
				{
				  // video to full range:
				  float Rfull = (float)((int)R1[pixel/3][line] - black)/Range;
				  float Gfull = (float)((int)G1[pixel/3][line] - black)/Range;
				  float Bfull = (float)((int)B1[pixel/3][line] - black)/Range;

					
				  float Red = PQ10000_f( Rfull);
				  float Grn = PQ10000_f( Gfull);
				  float Blu = PQ10000_f( Bfull);
				  
				  LMAX = max(Red,Grn);
				  LMAX = max(LMAX,Blu);
				  
		          if(R2020) {
			          // 2020
			          L = 0.2627*Red + 0.6780*Grn + 0.0593*Blu;				  
				  } else {
			          // P3D65:
					  L = 0.228975*Red + 0.691739*Grn + 0.0792869*Blu;
				  }
		          

		          
				  mean += L;
				  maxFALL += LMAX;
				  if(LMAX > maxCLL) maxCLL = LMAX;
                  //printf("writing line: %d,  pixel: %d, %f\n",line, pixel/3, Red);
									
				}
						
		
			}
			
         printf("%f, %f, %f\n",10000.0*(mean/(arraySizeX*arraySizeY)), 10000.0*(maxFALL/(arraySizeX*arraySizeY)), 10000.0*maxCLL);
         //printf("%s, %f, %f\n",argv[arg-1], 10000.0*(mean/(arraySizeX*arraySizeY)), 10000.0*(maxCLL/(arraySizeX*arraySizeY)));
         //printf("pixels: %d\n",arraySizeX*arraySizeY);

					
    }
    else {
    	cout << "\nerror\n";
    }
    
}




// Functions

// ctl script to implement BBC EOTF as described in public whitepaper
// WHP283

const float mu = 0.139401137752;
const float Lmax = 4.0;
const float nu = sqrt(mu)/2.0; // 0.186682308851
const float pho = sqrt(mu)*(1.0-log(sqrt(mu)));
const float eu = 0.3733646177;


float BBC_f(float V,float sf)
{
	float L;
	if (V <= eu)
	{
		L = pow(V, 2*sf);
	}
	else
	{
		L = exp(sf*(V-pho)/nu);
    }
	return L;
}

float BBC_r(float L, float sr)
{
	float V;
	if (L <= mu)
	{
		V = pow(L, 0.5/sr);
	}
	else
	{
		V = nu*log(L)/sr + pho;
	}
	return V;
}


float  BT1886_f(float V, float s)
{
	return pow (V,2*s);
}


float  BT1886_r(float L, float s)
{
	return pow(L,0.5/s);
}



// 10000 nits
//  1/gamma-ish, calculate V from Luma
// decode L = (max(,0)/(c2-c3*V**(1/m)))**(1/n)
double PQ10000_f( double V)
{
  double L;
  // Lw, Lb not used since absolute Luma used for PQ
  // formula outputs normalized Luma from 0-1
  L = pow(max(pow(V, 1.0/78.84375) - 0.8359375 ,0.0)/(18.8515625 - 18.6875 * pow(V, 1.0/78.84375)),1.0/0.1593017578);

  return L;
}

// encode (V^gamma ish calculate L from V)
//  encode   V = ((c1+c2*Y**n))/(1+c3*Y**n))**m
double PQ10000_r( double L)
{
  double V;
  // Lw, Lb not used since absolute Luma used for PQ
  // input assumes normalized luma 0-1
  V = pow((0.8359375+ 18.8515625*pow((L),0.1593017578))/(1+18.6875*pow((L),0.1593017578)),78.84375);
  return V;
}