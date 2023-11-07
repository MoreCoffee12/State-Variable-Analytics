/*13:*/
#line 975 "./sgfilter.w"

/*14:*/
#line 1002 "./sgfilter.w"

#include <math.h> 
#include <stdlib.h> 
#include <stdarg.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <sys/time.h> 
#include "sgfilter.h"

/*:14*/
#line 976 "./sgfilter.w"

/*15:*/
#line 1022 "./sgfilter.w"

extern char*optarg;
char*progname;

/*:15*/
#line 977 "./sgfilter.w"

/*22:*/
#line 1210 "./sgfilter.w"

/*23:*/
#line 1250 "./sgfilter.w"

void log_printf(const char*function_name,const char*format,...){
va_list args;
time_t time0;
struct tm lt;
struct timeval tv;
char logentry[1024];

gettimeofday(&tv,NULL);
time(&time0);
lt= *localtime(&time0);
sprintf(logentry,"%02u%02u%02u %02u:%02u:%02u.%03d ",
lt.tm_year-100,lt.tm_mon+1,lt.tm_mday,
lt.tm_hour,lt.tm_min,lt.tm_sec,tv.tv_usec/1000);
sprintf(logentry+strlen(logentry),"(%s) ",function_name);
va_start(args,format);
vsprintf(logentry+strlen(logentry),format,args);
va_end(args);
sprintf(logentry+strlen(logentry),"\n");
fprintf(stdout,"%s",logentry);
return;
}

/*:23*/
#line 1211 "./sgfilter.w"

/*24:*/
#line 1276 "./sgfilter.w"

int*ivector(long nl,long nh){
int*v;
v= (int*)malloc((size_t)((nh-nl+2)*sizeof(int)));
if(!v){
log("Error: Allocation failure.");
exit(1);
}
return v-nl+1;
}

/*:24*/
#line 1212 "./sgfilter.w"

/*25:*/
#line 1290 "./sgfilter.w"

double*dvector(long nl,long nh){
double*v;
long k;
v= (double*)malloc((size_t)((nh-nl+2)*sizeof(double)));
if(!v){
log("Error: Allocation failure.");
exit(1);
}
for(k= nl;k<=nh;k++)v[k]= 0.0;
return v-nl+1;
}

/*:25*/
#line 1213 "./sgfilter.w"

/*26:*/
#line 1307 "./sgfilter.w"

double**dmatrix(long nrl,long nrh,long ncl,long nch)
{
long i,nrow= nrh-nrl+1,ncol= nch-ncl+1;
double**m;
m= (double**)malloc((size_t)((nrow+1)*sizeof(double*)));
if(!m){
log("Allocation failure 1 occurred.");
exit(1);
}
m+= 1;
m-= nrl;
m[nrl]= (double*)malloc((size_t)((nrow*ncol+1)*sizeof(double)));
if(!m[nrl]){
log("Allocation failure 2 occurred.");
exit(1);
}
m[nrl]+= 1;
m[nrl]-= ncl;
for(i= nrl+1;i<=nrh;i++)m[i]= m[i-1]+ncol;
return m;
}

/*:26*/
#line 1214 "./sgfilter.w"

/*27:*/
#line 1333 "./sgfilter.w"

void free_ivector(int*v,long nl,long nh){
free((char*)(v+nl-1));
}

/*:27*/
#line 1215 "./sgfilter.w"

/*28:*/
#line 1341 "./sgfilter.w"

void free_dvector(double*v,long nl,long nh){
free((char*)(v+nl-1));
}

/*:28*/
#line 1216 "./sgfilter.w"

/*29:*/
#line 1350 "./sgfilter.w"

void free_dmatrix(double**m,long nrl,long nrh,long ncl,long nch){
free((char*)(m[nrl]+ncl-1));
free((char*)(m+nrl-1));
}

/*:29*/
#line 1217 "./sgfilter.w"

/*30:*/
#line 1372 "./sgfilter.w"

void lubksb(double**a,int n,int*indx,double b[])
{
int i,ii= 0,ip,j;
double sum;

for(i= 1;i<=n;i++){
ip= indx[i];
sum= b[ip];
b[ip]= b[i];
if(ii)
for(j= ii;j<=i-1;j++)sum-= a[i][j]*b[j];
else if(sum)ii= i;
b[i]= sum;
}
for(i= n;i>=1;i--){
sum= b[i];
for(j= i+1;j<=n;j++)sum-= a[i][j]*b[j];
b[i]= sum/a[i][i];
}
}

/*:30*/
#line 1218 "./sgfilter.w"

/*31:*/
#line 1407 "./sgfilter.w"

void ludcmp(double**a,int n,int*indx,double*d)
{
int i,imax= 0,j,k;
double big,dum,sum,temp;
double*vv;

vv= dvector(1,n);
*d= 1.0;
for(i= 1;i<=n;i++){
big= 0.0;
for(j= 1;j<=n;j++)
if((temp= fabs(a[i][j]))> big)big= temp;
if(big==0.0){
log("Error: Singular matrix found in routine ludcmp()");
exit(1);
}
vv[i]= 1.0/big;
}
for(j= 1;j<=n;j++){
for(i= 1;i<j;i++){
sum= a[i][j];
for(k= 1;k<i;k++)sum-= a[i][k]*a[k][j];
a[i][j]= sum;
}
big= 0.0;
for(i= j;i<=n;i++){
sum= a[i][j];
for(k= 1;k<j;k++)
sum-= a[i][k]*a[k][j];
a[i][j]= sum;
if((dum= vv[i]*fabs(sum))>=big){
big= dum;
imax= i;
}
}
if(j!=imax){
for(k= 1;k<=n;k++){
dum= a[imax][k];
a[imax][k]= a[j][k];
a[j][k]= dum;
}
*d= -(*d);
vv[imax]= vv[j];
}
indx[j]= imax;
if(a[j][j]==0.0)a[j][j]= EPSILON;
if(j!=n){
dum= 1.0/(a[j][j]);
for(i= j+1;i<=n;i++)a[i][j]*= dum;
}
}
free_dvector(vv,1,n);
}

/*:31*/
#line 1219 "./sgfilter.w"

/*32:*/
#line 1471 "./sgfilter.w"

#include <math.h> 
#define SWAP(a,b) tempr= (a);(a)= (b);(b)= tempr
void four1(double data[],unsigned long nn,int isign)
{
unsigned long n,mmax,m,j,istep,i;
double wtemp,wr,wpr,wpi,wi,theta;
double tempr,tempi;

n= nn<<1;
j= 1;
for(i= 1;i<n;i+= 2){
if(j> i){
SWAP(data[j],data[i]);
SWAP(data[j+1],data[i+1]);
}
m= n>>1;
while(m>=2&&j> m){
j-= m;
m>>= 1;
}
j+= m;
}
mmax= 2;
while(n> mmax){
istep= mmax<<1;
theta= isign*(6.28318530717959/mmax);
wtemp= sin(0.5*theta);
wpr= -2.0*wtemp*wtemp;
wpi= sin(theta);
wr= 1.0;
wi= 0.0;
for(m= 1;m<mmax;m+= 2){
for(i= m;i<=n;i+= istep){
j= i+mmax;
tempr= wr*data[j]-wi*data[j+1];
tempi= wr*data[j+1]+wi*data[j];
data[j]= data[i]-tempr;
data[j+1]= data[i+1]-tempi;
data[i]+= tempr;
data[i+1]+= tempi;
}
wr= (wtemp= wr)*wpr-wi*wpi+wr;
wi= wi*wpr+wtemp*wpi+wi;
}
mmax= istep;
}
}
#undef SWAP


/*:32*/
#line 1220 "./sgfilter.w"

/*33:*/
#line 1532 "./sgfilter.w"

void twofft(double data1[],double data2[],double fft1[],double fft2[],
unsigned long n)
{
void four1(double data[],unsigned long nn,int isign);
unsigned long nn3,nn2,jj,j;
double rep,rem,aip,aim;

nn3= 1+(nn2= 2+n+n);
for(j= 1,jj= 2;j<=n;j++,jj+= 2){
fft1[jj-1]= data1[j];
fft1[jj]= data2[j];
}
four1(fft1,n,1);
fft2[1]= fft1[2];
fft1[2]= fft2[2]= 0.0;
for(j= 3;j<=n+1;j+= 2){
rep= 0.5*(fft1[j]+fft1[nn2-j]);
rem= 0.5*(fft1[j]-fft1[nn2-j]);
aip= 0.5*(fft1[j+1]+fft1[nn3-j]);
aim= 0.5*(fft1[j+1]-fft1[nn3-j]);
fft1[j]= rep;
fft1[j+1]= aim;
fft1[nn2-j]= rep;
fft1[nn3-j]= -aim;
fft2[j]= aip;
fft2[j+1]= -rem;
fft2[nn2-j]= aip;
fft2[nn3-j]= rem;
}
}

/*:33*/
#line 1221 "./sgfilter.w"

/*34:*/
#line 1576 "./sgfilter.w"

void realft(double data[],unsigned long n,int isign)
{
void four1(double data[],unsigned long nn,int isign);
unsigned long i,i1,i2,i3,i4,np3;
double c1= 0.5,c2,h1r,h1i,h2r,h2i;
double wr,wi,wpr,wpi,wtemp,theta;

theta= 3.141592653589793/(double)(n>>1);
if(isign==1){
c2= -0.5;
four1(data,n>>1,1);
}else{
c2= 0.5;
theta= -theta;
}
wtemp= sin(0.5*theta);
wpr= -2.0*wtemp*wtemp;
wpi= sin(theta);
wr= 1.0+wpr;
wi= wpi;
np3= n+3;
for(i= 2;i<=(n>>2);i++){
i4= 1+(i3= np3-(i2= 1+(i1= i+i-1)));
h1r= c1*(data[i1]+data[i3]);
h1i= c1*(data[i2]-data[i4]);
h2r= -c2*(data[i2]+data[i4]);
h2i= c2*(data[i1]-data[i3]);
data[i1]= h1r+wr*h2r-wi*h2i;
data[i2]= h1i+wr*h2i+wi*h2r;
data[i3]= h1r-wr*h2r+wi*h2i;
data[i4]= -h1i+wr*h2i+wi*h2r;
wr= (wtemp= wr)*wpr-wi*wpi+wr;
wi= wi*wpr+wtemp*wpi+wi;
}
if(isign==1){
data[1]= (h1r= data[1])+data[2];
data[2]= h1r-data[2];
}else{
data[1]= c1*((h1r= data[1])+data[2]);
data[2]= c1*(h1r-data[2]);
four1(data,n>>1,-1);
}
}

/*:34*/
#line 1222 "./sgfilter.w"

/*35:*/
#line 1638 "./sgfilter.w"

char convlv(double data[],unsigned long n,double respns[],unsigned long m,
int isign,double ans[])
{
void realft(double data[],unsigned long n,int isign);
void twofft(double data1[],double data2[],double fft1[],double fft2[],
unsigned long n);
unsigned long i,no2;
double dum,mag2,*fft;

fft= dvector(1,n<<1);
for(i= 1;i<=(m-1)/2;i++)
respns[n+1-i]= respns[m+1-i];
for(i= (m+3)/2;i<=n-(m-1)/2;i++)
respns[i]= 0.0;
twofft(data,respns,fft,ans,n);
no2= n>>1;
for(i= 2;i<=n+2;i+= 2){
if(isign==1){
ans[i-1]= (fft[i-1]*(dum= ans[i-1])-fft[i]*ans[i])/no2;
ans[i]= (fft[i]*dum+fft[i-1]*ans[i])/no2;
}else if(isign==-1){
if((mag2= ans[i-1]*ans[i-1]+ans[i]*ans[i])==0.0){
log("Attempt of deconvolving at zero response in convlv().");
return(1);
}
ans[i-1]= (fft[i-1]*(dum= ans[i-1])+fft[i]*ans[i])/mag2/no2;
ans[i]= (fft[i]*dum-fft[i-1]*ans[i])/mag2/no2;
}else{
log("No meaning for isign in convlv().");
return(1);
}
}
ans[2]= ans[n+1];
realft(ans,n,-1);
free_dvector(fft,1,n<<1);
return(0);
}

/*:35*/
#line 1223 "./sgfilter.w"

/*36:*/
#line 1695 "./sgfilter.w"

char sgcoeff(double c[],int np,int nl,int nr,int ld,int m)
{
void lubksb(double**a,int n,int*indx,double b[]);
void ludcmp(double**a,int n,int*indx,double*d);
int imj,ipj,j,k,kk,mm,*indx;
double d,fac,sum,**a,*b;

if(np<nl+nr+1||nl<0||nr<0||ld> m||nl+nr<m){
log("Inconsistent arguments detected in routine sgcoeff.");
return(1);
}
indx= ivector(1,m+1);
a= dmatrix(1,m+1,1,m+1);
b= dvector(1,m+1);
for(ipj= 0;ipj<=(m<<1);ipj++){
sum= (ipj?0.0:1.0);
for(k= 1;k<=nr;k++)sum+= pow((double)k,(double)ipj);
for(k= 1;k<=nl;k++)sum+= pow((double)-k,(double)ipj);
mm= (ipj<2*m-ipj?ipj:2*m-ipj);
for(imj= -mm;imj<=mm;imj+= 2)a[1+(ipj+imj)/2][1+(ipj-imj)/2]= sum;
}
ludcmp(a,m+1,indx,&d);
for(j= 1;j<=m+1;j++)b[j]= 0.0;
b[ld+1]= 1.0;
lubksb(a,m+1,indx,b);
for(kk= 1;kk<=np;kk++)c[kk]= 0.0;
for(k= -nl;k<=nr;k++){
sum= b[1];
fac= 1.0;
for(mm= 1;mm<=m;mm++)sum+= b[mm+1]*(fac*= k);
kk= ((np-k)%np)+1;
c[kk]= sum;
}
free_dvector(b,1,m+1);
free_dmatrix(a,1,m+1,1,m+1);
free_ivector(indx,1,m+1);
return(0);
}

/*:36*/
#line 1224 "./sgfilter.w"

/*37:*/
#line 1775 "./sgfilter.w"

char sgfilter(double yr[],double yf[],int mm,int nl,int nr,int ld,int m){
int np= nl+1+nr;
double*c;
char retval;

#if CONVOLVE_WITH_NR_CONVLV 
c= dvector(1,mm);
retval= sgcoeff(c,np,nl,nr,ld,m);
if(retval==0)
convlv(yr,mm,c,np,1,yf);
free_dvector(c,1,mm);
#else 
int j;
long int k;
c= dvector(1,nl+nr+1);
retval= sgcoeff(c,np,nl,nr,ld,m);
if(retval==0){
for(k= 1;k<=nl;k++){
for(yf[k]= 0.0,j= -nl;j<=nr;j++){
if(k+j>=1){
yf[k]+= c[(j>=0?j+1:nr+nl+2+j)]*yr[k+j];
}
}
}
for(k= nl+1;k<=mm-nr;k++){
for(yf[k]= 0.0,j= -nl;j<=nr;j++){
yf[k]+= c[(j>=0?j+1:nr+nl+2+j)]*yr[k+j];
}
}
for(k= mm-nr+1;k<=mm;k++){
for(yf[k]= 0.0,j= -nl;j<=nr;j++){
if(k+j<=mm){
yf[k]+= c[(j>=0?j+1:nr+nl+2+j)]*yr[k+j];
}
}
}
}
free_dvector(c,1,nr+nl+1);
#endif
return(retval);
}

/*:37*/
#line 1225 "./sgfilter.w"

/*38:*/
#line 1830 "./sgfilter.w"

/*39:*/
#line 1844 "./sgfilter.w"

short pathcharacter(int ch){
return(isalnum(ch)||(ch=='.')||(ch=='/')||(ch=='\\')||(ch=='_')||
(ch=='-')||(ch=='+'));
}

/*:39*/
#line 1831 "./sgfilter.w"

char*strip_away_path(char filename[]){
int j,k= 0;
while(pathcharacter(filename[k]))k++;
j= (--k);
while(isalnum((int)(filename[j])))j--;
j++;
return(&filename[j]);
}

/*:38*/
#line 1226 "./sgfilter.w"

/*40:*/
#line 1854 "./sgfilter.w"

/*41:*/
#line 1908 "./sgfilter.w"

void hl(const char*format,...){
va_list args;
char line[1024];

va_start(args,format);
vsprintf(line,format,args);
va_end(args);
sprintf(line+strlen(line),"\n");
fprintf(stdout,"%s",line);
return;
}

/*:41*/
#line 1855 "./sgfilter.w"

void showsomehelp(void){
hl("Usage: %s [options]",progname);
hl("Options:");
hl(" -h, --help");
hl("    Display this help message and exit clean.");
hl(" -i, --inputfile <str>");
hl("    Specifies the file name from which unfiltered data is to be read.");
hl("    The input file should describe the input as two columns contain-");
hl("    ing $x$- and $y$-coordinates of the samples.");
hl(" -o, --outputfile <str>");
hl("    Specifies the file name to which filtered data is to be written,");
hl("    again in a two-column format containing $x$- and $y$-coordinates");
hl("    of the filtered samples. If this option is omitted, the generated");
hl("    filtered data will instead be written to the console (terminal).");
hl(" -nl <nl>");
hl("    Specifies the number of samples nl to use to the 'left' of the");
hl("    basis sample in the regression window (kernel). The total number");
hl("    of samples in the window will be nL+nR+1.");
hl(" -nr <nr>");
hl("    Specifies the number of samples nr to use to the 'right' of the");
hl("    basis sample in the regression window (kernel). The total number");
hl("    of samples in the window will be nL+nR+1.");
hl(" -m <m>");
hl("    Specifies the order m of the polynomial to use in the regression");
hl("    analysis leading to the Savitzky-Golay coefficients. Typical");
hl("    values are between m=2 and m=6. Beware of too high values, which");
hl("    easily makes the regression too sensitive, with an oscillatory");
hl("    result.");
hl(" -ld <ld>");
hl("    Specifies the order of the derivative to extract from the ");
hl("    Savitzky--Golay smoothing algorithm. For regular Savitzky-Golay");
hl("    smoothing of the input data as such, use ld=0. For the Savitzky-");
hl("    Golay smoothing and extraction of derivatives, set ld to the");
hl("    order of the desired derivative and make sure that you correctly");
hl("    interpret the scaling parameters as described in 'Numerical");
hl("    Recipes in C', 2nd Edn (Cambridge University Press, New York,");
hl("    1994).");
hl(" -v, --verbose");
hl("    Toggle verbose mode. (Default: Off.)  This option should always");
hl("    be omitted whenever no  output file has been specified (that is");
hl("    to say, omit any --verbose or -v option whenever --outputfile or");
hl("    -o has been omitted), as the verbose logging otherwise will");
hl("    contaminate the filtered data stream written to the console");
hl("    (terminal).");
}

/*:40*/
#line 1227 "./sgfilter.w"

/*42:*/
#line 1925 "./sgfilter.w"

long int num_coordinate_pairs(FILE*file){
double tmp;
int tmpch;
long int mm= 0;
fseek(file,0L,SEEK_SET);
while((tmpch= getc(file))!=EOF){
ungetc(tmpch,file);
fscanf(file,"%lf",&tmp);
fscanf(file,"%lf",&tmp);
mm++;
tmpch= getc(file);
while((tmpch!=EOF)&&(!isdigit(tmpch)))tmpch= getc(file);
if(tmpch!=EOF)ungetc(tmpch,file);
}
fseek(file,0L,SEEK_SET);
return(mm);
}

/*:42*/
#line 1228 "./sgfilter.w"


/*:22*/
#line 978 "./sgfilter.w"


int main(int argc,char*argv[])
{
/*16:*/
#line 1046 "./sgfilter.w"

int no_arg;
int nl= DEFAULT_NL;
int nr= DEFAULT_NR;
int ld= DEFAULT_LD;
int m= DEFAULT_M;
long int k,mm= 0;
double*x,*yr,*yf;
char input_filename[NCHMAX]= "",output_filename[NCHMAX]= "";
char verbose= 0;
FILE*file;

/*:16*/
#line 982 "./sgfilter.w"

/*17:*/
#line 1064 "./sgfilter.w"

progname= strip_away_path(argv[0]);
no_arg= argc;
while(--argc){
if(!strcmp(argv[no_arg-argc],"-o")||
!strcmp(argv[no_arg-argc],"--outputfile")){
--argc;
strcpy(output_filename,argv[no_arg-argc]);
}else if(!strcmp(argv[no_arg-argc],"-i")||
!strcmp(argv[no_arg-argc],"--inputfile")){
--argc;
strcpy(input_filename,argv[no_arg-argc]);
}else if(!strcmp(argv[no_arg-argc],"-h")||
!strcmp(argv[no_arg-argc],"--help")){
showsomehelp();
exit(0);
}else if(!strcmp(argv[no_arg-argc],"-v")||
!strcmp(argv[no_arg-argc],"--verbose")){
verbose= (verbose?0:1);
}else if(!strcmp(argv[no_arg-argc],"-nl")){
--argc;
if(!sscanf(argv[no_arg-argc],"%d",&nl)){
log("Error in '-nl' option.");
exit(1);
}
}else if(!strcmp(argv[no_arg-argc],"-nr")){
--argc;
if(!sscanf(argv[no_arg-argc],"%d",&nr)){
log("Error in '-nr' option.");
exit(1);
}
}else if(!strcmp(argv[no_arg-argc],"-ld")){
--argc;
if(!sscanf(argv[no_arg-argc],"%d",&ld)){
log("Error in '-ld' option.");
exit(1);
}
}else if(!strcmp(argv[no_arg-argc],"-m")){
--argc;
if(!sscanf(argv[no_arg-argc],"%d",&m)){
log("Error in '-m' option.");
exit(1);
}
}else{
log("Unrecognized option '%s'.",argv[no_arg-argc]);
showsomehelp();
exit(1);
}
}
if(verbose)fprintf(stdout,"This is %s v.%s. %s\n",progname,VERSION,COPYRIGHT);

/*:17*/
#line 983 "./sgfilter.w"

/*18:*/
#line 1121 "./sgfilter.w"

if(!strcmp(input_filename,"")){
log("No input file specified! (Please use the '--inputfile' option.)");
log("Execute '%s --help' for help.",progname);
exit(1);
}
if((file= fopen(input_filename,"r"))==NULL){
log("Could not open %s for loading raw data!",input_filename);
exit(1);
}
mm= num_coordinate_pairs(file);
if(mm<nl+nr+1){
log("Error: The number M=%ld of data points must be at least nl+nr+1=%d",
mm,nl+nr+1);
log("Please check your -nl or -nr options.");
exit(1);
}
if(verbose){
log("Loading %ld unfiltered samples from %s...",mm,input_filename);
log(" ... allocating memory for storage ...");
}
x= dvector(1,mm);
yr= dvector(1,mm);
#if CONVOLVE_WITH_NR_CONVLV
yf= dvector(1,2*mm);
#else
yf= dvector(1,mm);
#endif
if(verbose)
log(" ... scanning %s for input data ...",input_filename);
for(k= 1;k<=mm;k++){
fscanf(file,"%lf",&x[k]);
fscanf(file,"%lf",&yr[k]);
}
fclose(file);
if(verbose)
log(" ... done. Input now residing in RAM.");

/*:18*/
#line 984 "./sgfilter.w"

/*19:*/
#line 1162 "./sgfilter.w"

if(!sgfilter(yr,yf,mm,nl,nr,ld,m)){
if(verbose)
log("Successfully performed Savitzky-Golay filtering.");
}else{
if(verbose)
log("Error: Could not perform Savitzky-Golay filtering.");
}

/*:19*/
#line 985 "./sgfilter.w"

/*20:*/
#line 1178 "./sgfilter.w"

if(!strcmp(output_filename,"")){
if(verbose)
log("Writing %ld filtered samples to console...",mm);
}else{
if(verbose)
log("Writing %ld filtered samples to %s...",mm,output_filename);
if((file= freopen(output_filename,"w",stdout))==NULL){
log("Error: Unable to redirect stdout stream to file %s.",
output_filename);
exit(1);
}
}
for(k= 1;k<=mm;k++)fprintf(stdout,"%1.8f %1.8f\n",x[k],yf[k]);
#ifdef UNIX_LIKE_OS
freopen("/dev/tty","a",stdout);
#endif
if(verbose)log(" ... done.");

/*:20*/
#line 986 "./sgfilter.w"

/*21:*/
#line 1199 "./sgfilter.w"

free_dvector(x,1,mm);
free_dvector(yr,1,mm);
#if CONVOLVE_WITH_NR_CONVLV
free_dvector(yf,1,2*mm);
#else
free_dvector(yf,1,mm);
#endif

/*:21*/
#line 987 "./sgfilter.w"

return(EXIT_SUCCESS);
}

/*:13*/
