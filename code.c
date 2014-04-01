//下面的函数用以检测视频中运动的目标。基于背景差模型
 void CtraceDlg::OnBnClickedTracing() {  IplImage* pFrame=NULL;//声明当前帧 
 IplImage* pFrImg=NULL;//声明前景图像
IplImage* pBkImg=NULL;//声明背景图像 
//声明对应的图像矩阵  
CvMat* pFrameMat=NULL;  CvMat* pFrMat=NULL;  CvMat* pBkMat=NULL; 
 CvCapture* pCapture=NULL; //声明视频捕捉器，用以读取视频内容 
 int nFrmNum=0; 
 if(!(pCapture=cvCaptureFromFile(aviFilePath))) 
 {   MessageBox(_T("请先打开视频文件"));   return; 
 } 
 //create video windows 
 cvNamedWindow("Video",1); 
 cvNamedWindow("Background",1);  
cvNamedWindow("Foreground",1); 
cvMoveWindow("Video", 30, 0);     
cvMoveWindow("Background", 360, 0);     
cvMoveWindow("Foreground", 690, 0);   //read every frame 
 while(pFrame=cvQueryFrame(pCapture)) 
 {   nFrmNum++;   if(nFrmNum==1)   {  
 
//存放背景图像（灰度）
pBkImg = cvCreateImage(cvSize(pFrame->width, 
pFrame->height), 
IPL_DEPTH_8U,1);    
//存放前景图像（灰度）  
 pFrImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);   //图像对应的矩阵都是一维的 
 pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);  
pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);             
pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);    //将彩色图像转化为灰度图像 
cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);   
 cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);    //将灰度图像存入矩阵，灰度图像是单通道的    
cvConvert(pFrImg, pFrameMat);    
cvConvert(pFrImg, pFrMat);    
cvConvert(pBackground, pBkMat); 
  }  
 else  
 {    cvCvtColor(pFrame,pFrImg,CV_BGR2GRAY);    cvConvert(pFrImg,pFrameMat);    //高斯滤波 
  cvSmooth(pFrameMat,pFrameMat,CV_GAUSSIAN,3,0,0,0);    //当前帧减去背景图像并取绝对值    
cvAbsDiff(pFrameMat,pBkMat,pFrMat);    //二值化前景图像 
cvThreshold(pFrMat, pFrImg, 20, 255.0, CV_THRESH_BINARY);    //形态学滤波，去噪 
cvErode(pFrImg, pFrImg, 0, 1);    
cvDilate(pFrImg, pFrImg, 0, 2);   
cvErode(pFrImg,pFrImg,0,1);    //滑动平均更新背景（求平均） 
cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);  
 //将背景矩阵转化为图像格式，用以显示
   cvConvert(pBkMat,pBkImg);    //保持图像的旋转方向 
   //pBkImg->origin=pFrImg->origin=pFrame->origin;    //显示图像 
cvShowImage("Video",pFrame); 
 cvShowImage("Background",pBackground);    
cvShowImage("Foreground",pFrImg);   
 if(cvWaitKey(200)>=0)     
break; 
  } 
 } 
 cvDestroyWindow("Video");  
cvDestroyWindow("Background"); 
 cvDestroyWindow("Foreground");   
cvReleaseImage(&pFrImg);  
cvReleaseImage(&pBkImg);   
cvReleaseMat(&pFrameMat);  
cvReleaseMat(&pFrMat);  
cvReleaseMat(&pBkMat);   
cvReleaseCapture(&pCapture); } 
//下面的函数用来跟踪视频中的目标 
void CtraceDlg::OnBnClickedButton2() 
{  CvCapture* pCapture=cvCaptureFromFile(aviFilePath);//视频捕捉器  
CvMat* foreImageMat=NULL;//前景矩阵  
CvMat* pBackgroundMat=NULL;  
CvMat* pMat=NULL; 
IplImage* pFrame=NULL,*pGray=NULL;  
pFrame=cvQueryFrame(pCapture); 
if(pFrame==NULL ||  pBackground==NULL)
{MessageBox(_T("请先打开视频文件并训
练图像背景"));return;} 
 int width=pFrame->width,height=pFrame->height;  
pBackgroundMat=cvCreateMat(height,width,CV_32FC1);  
pMat=cvCreateMat(height,width,CV_32FC1);  
foreImageMat=cvCreateMat(height,width,CV_32FC1); 
 previousLabeledMat=cvCreateMat(height,width,CV_32FC1);//前帧标签矩阵  
currentLabeledMat=cvCreateMat(height,width,CV_32FC1);//当前帖标签矩阵  
pGray=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);  
previousObjectSet=new movingObject();//前帧目标单链表  
currentObjectSet=new movingObject();//当前帧目标单链表  
cvCvtColor(pFrame,pGray,CV_BGR2GRAY);//将当前帧灰度化  
cvSmooth(pGray,pGray,CV_GAUSSIAN,3);//高斯滤波 
cvAbsDiff(pGray,pBackground,pGray);//与背景作差并求绝对值 
 cvThreshold(pGray,pGray,20,255,CV_THRESH_BINARY);//对前景图像二值化  
cvErode(pGray,pGray,0,1);//腐蚀  cvDilate(pGray,pGray,0,1);//膨胀 
 cvConvert(pGray,foreImageMat);//将前景图像转换为矩阵 
 computeObject(foreImageMat,width,height,previousLabeledMat,previousObjectSet);//调用函数计算各目标  
movingObject*p=previousObjectSet->next,*q=NULL;//定义遍历目标单链表的临时指针  
objectList *t1=NULL,*t2=NULL;//定义遍历目标轨迹的临时指针  
char* data=pFrame->imageData; 
int step=pFrame->widthStep/sizeof(uchar);  while(p!=NULL)  {   
data[(p->y)*step+(p->x)*3+1]=255; 
 cvRectangle(pFrame,cvPoint(p->x-10,p->y10),cvPoint(p->x+10,p->y+10),cvScalar(0,255,0),1,8,0);   p=p->next; 
 } 
 cvNamedWindow("test",1); 
cvNamedWindow("frame",1); 
cvShowImage("test",pFrame); 
pFrame=cvQueryFrame(pCapture);  
float tmp_x,tmp_y;int tmp_label;  
while(pFrame!=NULL) 
 {   cvCvtColor(pFrame,pGray,CV_BGR2GRAY);
cvShowImage("frame",pFrame);    
cvConvert(pBackground,pBackgroundMat);    
cvSmooth(pGray,pGray,CV_GAUSSIAN,3);   
cvConvert(pGray,pMat); 
cvRunningAvg(pMat,pBackgroundMat,0.003,0);   
cvAbsDiff(pMat,pBackgroundMat,pMat);   
cvConvert(pMat,pGray); 
  cvThreshold(pGray,pGray,20,255,CV_THRESH_BINARY);   
cvErode(pGray,pGray,0,1);   
cvDilate(pGray,pGray,0,2);   
cvErode(pGray,pGray,0,1);   
cvConvert(pGray,foreImageMat); 
computeObject(foreImageMat,width,height,currentLabeledMat,currentObjectSet)
;//MessageBox(_T("success here"));   
q=currentObjectSet->next;   data=pFrame->imageData; 
 step=pFrame->widthStep/sizeof(uchar);   
while(q!=NULL)   {    //处理每个目标的轨迹    
p=previousObjectSet->next;    
if(p!=NULL)    { 
tmp_x=sqrt(pow((float)( 
p->x-q->x),2)+pow((float)(p->y-q->y),2)); 
tmp_label=p->label;
 } 
p=p->next;    while(p!=NULL)    { 
 if( sqrt(pow((float)( p->x-q->x),2)+pow((float)(p->y-q->y),2))<tmp_x)     { 
 tmp_x=sqrt(pow((float)( 
p->x-q->x),2)+pow((float)(p->y-q->y),2)); 
tmp_label=p->label;     
} 
 p=p->next; 
 } 
  p=previousObjectSet->next;    while(p!=NULL)    {     if(p->label==tmp_label && abs(p->x-q->x)<100)     {      q->head->next=p->head;      p->head=NULL;      break;     } 
  p=p->next; 
  } 
  t1=q->head;    while(t1!=NULL)    { 
 cvRectangle(pFrame,cvPoint(t1->x-1,t1->y-2),cvPoint(t1->x+1,t1->y+2),cvScal
ar(255,0,0),4,8,0);     t1=t1->next; 
  } 
 //cvLine(pFrame,cvPoint(q->head->x,q->head->y),cvPoint(t1->x,t1->y),cvScalar(255,0,0),6,8,0);     data[q->y*step+q->x*3+1]=255;     int a=(int)sqrt((double)q->area);    
a=a/2; 
 cvRectangle(pFrame,cvPoint(q->x-a,q->y-a),cvPoint(q->x+a,q->y+a),cvScalar(0
,255,0),1,8,0);    q=q->next; 
  }    
  cvShowImage("test",pFrame);   if(cvWaitKey(200)>0)break;   pFrame=cvQueryFrame(pCapture); 
  //前帧的目标链表清除，再指向当前帧的目标链表   
p=previousObjectSet->next;   while(p!=NULL)   {   
q=p; p=p->next;  
      t1=q->head; q->head=NULL;       while(t1!=NULL)       {    
   t2=t1; 
       t1=t1->next;    
   delete t2; 
       }    delete q; 
  } 
  previousObjectSet->next=currentObjectSet->next;   currentObjectSet->next=NULL; 
 } 
 cvWaitKey(0); 
 cvReleaseCapture(&pCapture); 
cvReleaseMat(&previousLabeledMat);  
cvReleaseMat(&currentLabeledMat);  
cvReleaseImage(&pGray);  
cvDestroyWindow("test");  
p=previousObjectSet;  
while(p!=NULL)  {   q=p;  p=p->next;   delete q; 
 } 
 p=currentObjectSet;  while(p!=NULL)  {   q=p;   p=p->next;   delete q;  } 
}   
//对二值化的前景矩阵进连通分量标记，并返回一个结构数组。该结构包含区域面积、质心坐标及包 
//含本区域的矩形的宽高分量 void 
computeObject(CvMat* 
foreImageMat,int 
width,int 
height, 
CvMat* 
labeledMat,movingObject* head) {  int neibor[8][2]={ {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };  int index=0,row,col,tRow,tCol;  cvSetZero(labeledMat);  CvPoint points[50000];  int top=0; int* pmArr=NULL;  //标记连通分量 
 for(int i=0;i<height;i++)  
for(int j=0;j<width;j++){    if(cvmGet(foreImageMat,i,j)==255 && cvmGet(labeledMat,i,j)==0)    {     index++; 
   points[top].x=j; points[top].y=i;     top++;     while(top>0)     {      top-;  row=points[top].y; col=points[top].x;      
cvmSet(labeledMat,row,col,index);      
for(int m=0;m<8;m++)      
{       
tRow=row+neibor[m][0]; tCol=col+neibor[m][1];       
if(tRow>=0 && tRow<height && tCol>=0 && tCol<width)  
if(cvmGet(foreImageMat,tRow,tCol)==255 
&& 
cvmGet(labeledMat,tRow,tCol)==0)        
{         
points[top].x=tCol; points[top].y=tRow;         top++; 
      } 
     } 
    } 
   } 
  
} 
 //统计运动目标  
pmArr=new int[index]; 
 int *mx0Arr=new int[index];int *my0Arr=new int[index];  int value; 
 for(int i=0;i<index;i++)  {   pmArr[i]=0; mx0Arr[i]=0; my0Arr[i]=0; 
 } 
 for(int i=0;i<height;i++)  
 for(int j=0;j<width;j++) 
  {    
value=(int)cvmGet(labeledMat,i,j);    if(value!=0)   
 {     
pmArr[value-1]++; mx0Arr[value-1]+=j; my0Arr[value-1]+=i; 
   } 
  
} 
 movingObject *p=NULL, *t=NULL;  p=head->next;  if(p!=NULL)  {   t=p; p=p->next; 
  objectList *t1=NULL,*t2=NULL;   t1=t->head; t->head=NULL;   while(t1!=NULL)   {    t2=t1;  
 t1=t1->next; 
 
 
delete t2; 
  }   delete t; 
 } 
 p=head; p->next=NULL; t=NULL;  for(int i=0;i<index;i++)  {   if(pmArr[i]>100)   {  
t=new 
movingObject(); 
 
t->area=pmArr[i]; 
t->label=i; 
t->x=mx0Arr[i]/pmArr[i]; t->y=my0Arr[i]/pmArr[i]; t->next=NULL;  
 
t->head=new 
objectList(); t->head->x=t->x; t->head->y=t->y; 
t->head->next=NULL;   
p->next=t;  
 p=p->next; 
  } 
 } 
 delete[] pmArr; delete[] mx0Arr; delete[] my0Arr; 
}
