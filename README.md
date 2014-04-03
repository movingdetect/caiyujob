基于背景差分的运动图像检测报告
                                                       蔡宇 Z0712030727
1．引言 
运动目标的检测与跟踪是视觉监控系统等应用领域的重要研究内容。随着视频信息智能化处理需求的不断增加，如何从视频流中提取运动目标、并对目标进行实时跟踪越来越受到人们的关注。 
减背景方法是常用的运动目标检测方法。其基本思想是将视频流中当前一帧所有像素点与事先通过某种方法计算得到的背景图像中对应像素点相减并取绝对值，如果绝对值超过某个预先设定好的阀值，则认为当前帧中对应的像素点是运动目标的像素点；否则，就认为对应的像素点是背景像素点。相减运算的结果还提供了视频流中运动目标的位置、大小及形状等信息。但是该方法在应用过程中常会遇到如下的问题： 
（1）背景获取：最简单的背景获取方法就是在视频场景没有运动目标的情况下直接将某一帧存储为背景图像，但在大多数的视频应用中，如交通监控和行人检测，这一要求很难得到满足，故需要一种能够在运动目标存在的视频流中实时获取背景图像的方法。 
（2）背景扰动：背景中经常存在一些对象轻微的扰动，如树枝的摇动，扰动部分不应该看作是前景运动目标。 
（3）光照变化：天气、光线等因素随着时间的变化也会影响运动目标的检测结果，这是必须要考虑的问题。 
（4）背景更新：为了适应外界各种条件的不断变化，有必要对建立起来的背景模型进行实时更新。 
以往的运动目标检测方法或者不能解决以上所有问题，或者是通过构造复杂的模型来解决以上问题，其计算复杂性和对系统的要求都比较高，有时可能无法满足实时处理的要求。    本文在减背景方法的基础上，提出了一种更为有效的运动 目标检测方法。在背景的提取阶段，允许视频流中有运动目标的存在，在这种情况下，首先采用基于统计的方法建立背景模型，然后进行减背景操作来检测视频中的运动目标，并对背景模型进行实时更新，以适应光线的变化和场景本身的变化，最后对检测结果使用形态学运算和连通区域面积限制目标大小的方法进行后处理，消除噪声和背景扰动带来的影响。当运动目标确定后，采用区域跟踪技术对目标进行实时跟踪，跟踪技术中使用了两个参数以避免运动目标之间的遮挡问题。实验结果证明，提出的方法取得了比较理想的结果。   
 
2 视频中运动目标的检测过程    
2.1 背景模型的建立 
背景模型建立的准确与否，直接关系到运动目标检测结果的准确性。国内外已经提出了许多背景模型的建立方案，如W4方法，该方法是在视频中没有目标出现的情况下，对场景进行一段时间的测量，记录每个像素点的最大和最小亮度值，以及相邻两帧间亮度最大差异值，然后使用这3个值表示背景模型。还有使用前几帧图像像素亮度值的平均值作为背景模型的。以上这些方法都要求视频中至少一段时间内没有运动目标的出现，但是这种要求在多数场合下并不能得到满足。  
本文提出一种简单、有效的背景模型建立方法，它可以在视频场景中存在运动目标的情况下提取出背景图像。此方法是基于这样的假设，在背景模型建立阶段，运动目标虽然在场景区域中运动 ，但是它并不会长时问地停留在某一位置上。
 
图1
对视频流中某一像素点进行一段时间的观测，可以发现，它的亮度值只是在前景运动目标通过该点时，才会发生较大的变化，如图1(a)所示。对该像素点的亮度值进行统计，可以看出，在一段时间内，它的亮度值主要集中在一个很小的区域中，如图1(b)所示。因此，可以用这个区域内的平均值作为该点的背景值。 
具体实现过程如下：在灰度模式下，像素亮度值的变化范围为0 ~ 255，将该范围划分成若干区间[ 0，T]，[T，2T]，[nT，255]，n = 255/T。对于每个像素点，统计一段视频内每个区间亮度值出现的次数，找出出现次数最多的那个区间，接着计算该区间内所有亮度值的平均值，用该平均值作为背景图像在该点的亮度值。实验表明，该方法提取背景过程的中，不受前景运动目标的影响。 
 1.2运动目标检测 
运动目标检测是计算视频流中当前帧的像素点和背景图像中对应像素点的差异并取其绝对值，如果绝对值大于给定的阈值，则判定该像素点为前景运动目标的像素点。一对像素点在做相减运算时，可以使用它们的亮度值、色度值或其它参数，本文采用亮度值相减，检测规则如下： 
𝑀 𝑥,𝑦 = 0         𝑖𝑓  𝐼 𝑥,𝑦 −𝐵 𝑥,𝑦  <𝑇𝐻1        𝑖𝑓  𝐼 𝑥,𝑦 −𝐵 𝑥,𝑦  ≥𝑇𝐻（1） 
其中𝑀 𝑥,𝑦 是运动目标像素点的集合，𝐼 𝑥,𝑦 表示当前帧中的像素点亮度值，𝐵 𝑥,𝑦 表示背景图像中对应像素点的亮度值，𝑇𝐻为阀值，目前阀值的选择主要依靠先验知识，比如根据目标与背景的亮度差别等。  
1.3 后处理及背景模型的更新 
由于视频本身不可避免的噪声影响，会使上述检测结果出现一些问题，比如本是背景图像的区域像素点经过检测后被当成运动区域的像素点，运动目标内的部分区域因为某种原因被漏检，以及背景图像的分部区域由于树枝、树叶的轻微晃动而被误判为运动目标的区域等等。为了消除这些影响，首先对上一步的检测结果用形态学的腐蚀、膨胀方法进行处理，再找出其经过形态学处理后的连通区域，接着计算每个连通区域的面积，对于面积小于一定阀值的区域，将其抛弃，不看作是前景运动目标区域。形态学运算使用5X5的矩阵算子，连通区域内像素点个数的阈值选择要视具体情况而定。 
背景模型的实时更新是要使背景能够对外界环境的变化具有一定的自适应性。对于光线的变化，我们的方法与文献[6]中采用的方法类似。对于检测到的存在运动目标的区域不进行更新，而只对没有检测到运动目标的区域进行实时更新，对于这部分区域： 
𝐵n+1 𝑥,𝑦 =𝛼𝐵n 𝑥,𝑦 + 1−𝛼 𝐼n 𝑥,𝑦     （2）   
在具体的检测过程中，有时前景与背景对应像素点的亮度值相差很小，导致前景目标的部分区域被漏检掉，如果对背景中对应部分也进行更新，就会生成错误的背景模型，对后面的检测过程造成一定的影响，因此，选择最近N帧没有前景目标通过的像素点进行更新 。具体更新过程如下 ：  𝐵n+1 𝑥,𝑦 =𝛼𝐵n 𝑥,𝑦 + 1−𝛼 
𝐼n-N 𝑥,𝑦 +𝐼n-N+1 𝑥,𝑦 +⋯+𝐼n(𝑥,𝑦)
𝑁（3） 
其中𝐵n 𝑥,𝑦 ， 𝐵n+1 𝑥,𝑦 分别代表当前和下一帧背景，𝐼n-N 𝑥,𝑦 ，𝐼n(𝑥,𝑦)分别代表该点最近的N个亮度值，𝛼∈(0,1)为更新系数，控制背景更新的速度。 
图2是一个像素点亮度值的观测曲线，可以看出，该方法对于光线变化有很强的自适应性。  
 
图2
3运动物体的跟踪 
当目标检测出来以后，就要对运动的目标进行跟踪，本文采用区域跟踪的方法实现对运动目标的跟踪, 并在区域跟踪算法中选用了二个参数实现匹配。该方法能够在物体重叠和遮挡的情况下准确地定位出运动物体在图像中的位置。很好地实现了物体的跟踪, 也满足了实时性处理的要求。 
图像中标号为I的目标的大小用目标面积 SizeI来描述,每一目标都对应一个外截矩形框，用 BoxI来进行描述。Box可用一条对角线的两端点 S , T的坐标来表示: 
𝐵𝑜𝑥 𝑆,𝑇 ,     𝑆= 𝑠1,𝑠2 ,𝑇= 𝑡1,𝑡2 ,    𝑠𝑖≤𝑡𝑖  𝑖=1,2       (4) 
定义1:相邻两帧中标号为 i的目标与标号为 j 的目标面积大小差: 𝐷𝑖𝑓 𝑆𝑖𝑧𝑒𝐼 𝐾 ,𝑠𝑖𝑧𝑒𝐽 𝐾+1  = 𝑆𝑖𝑧𝑒𝐼 𝐾 −𝑆𝑖𝑧𝑒𝐽 𝐾+1  , 𝑖,𝑗 =1,2,…,𝑁    (5) 
其中 N 为图像中目标的标号。 
定义2:标号为L的目标的质心定义如下 
𝐶𝑡𝑟𝐿= 𝑋,𝑌 ,   𝑋= 𝑥i𝑁
𝑖=1
,   𝑌= 𝑦i𝑁
𝑖=1
      (6)  
这里N是图像中标号为L的目标含有象素点个数, xi是象素点i的横坐标, yi是象素点i的纵坐标。 
相邻两帧中标号为 I的目标与标号为J的目标质心之间的距离 
𝐷𝑖𝑠 𝐶𝑡𝑟𝐼 𝐾 ,𝐶𝑡𝑟𝐽 𝐾+1  =  𝑋I−𝑋J 2+ 𝑌I−𝑌j 2     (7) 具体跟踪步骤如下： 
(1)将第一帧中的目标检测出来，并计算出被跟踪目标(假设为目标M)在本帧中面积的大小、质心的位置等数据。 
(2)检测出下一帧中的目标 ，计算出各自的面积以及质心的位置。 (3)分别计算出目标M与下一帧中各目标的质心间的距离Dis和面积差Dif 𝐼𝑓 𝛼∗𝐷𝑖𝑠 𝐶𝑡𝑟𝑀 𝐾 ,𝐶𝑡𝑟𝑖 𝐾+1  +𝛽∗𝐷𝑖𝑓 𝑆𝑖𝑧𝑒𝑀 𝐾 ,𝑆𝑖𝑧𝑒𝑖 𝐾+1  ≤𝛿     (8) Then i就是M 
这里𝛿是阀值，𝛼，𝛽是加权系统，i为下一帧中各目标的标号。 
 (4)更新被跟踪车辆 M 的面积和质心位置的数据。转到第(2)步继续实现对后续帧的跟踪。 
运用上述的跟踪算法就可以实现对某一目标的跟踪, 也能够实现对多目标的同时跟踪。以上算法是建立在这样的假设条件下的：由于两相邻帧的时间间隔很短，因而在相邻两帧之间目标的移动距离并不大, 并且目标在图像中的面积变化率不大。实验表明这种假设是完全满足的, 能很好地实现物体的跟踪。
当两个物体在场景中相重叠时需要首先将它们分开后才能够用上述的算法进行跟踪。
4实验结果及结论 
我们采用一段交通视频来验证提出的模型，编程环境选用Opencv2.1和Visual C++ 2010。Opencv2.1是一种开源的用于图像处理和计算机视觉的函数库，由Intel公司使用C++高级语言开发。Visual C++ 2010是微软公司开发的编程工具，支持最新的C++0x标准。图3是实验的结果，其中图3a是原始图像，图3b是检测和跟踪结果，检测方框可随着目标的大小而改变，目标的运动轨迹由同一目标在各帧中的质心组成，在每一帧中各个目标的质心由实心圆代表。   
                   
图3
提出的背景模型在提取过程中允许运动目标存在，因此需要20~30秒的时间来完成背景模型的建立。通过对背景模型的实时更新，可以适应光线、天气等外界条件变化带来的影响。这种方法能够在运动物体频繁出现的场合下获得满意的效果，然后运用质心间的距离与物体面积之差两个参数对检测出来的目标进行实时跟踪。从上述实验结果中可以看到，该方法快速、准确，有着广泛的适用性。 
还可以在上述基础上，进行目标识别、流量统计等其它操作，这需要进一步的研究工作。方法还存在一些问题，第1个问题是当前景运动目标与背景亮度很接近时，运动目标很难被检测出来，或者是运动目标出现较大的空洞，对于这种 情况，是因为在判断运动目标时，仅仅使用了亮度信息，一个解决的途径是使用颜色与轮廓及深度等相结合的方法。第2个问题是如何实时更新背景来适应场景光线的突变，如室内开灯、关灯的情况，目前还没有比较有效的解决方法，有人采用事先存储有代表性的背景的方法，但这种方法对于场景本身发生变化的情况，明显无法适应。因此，这个问题需要在后续工作中进一步深入研究。


















附录1  源代码（模块代码主体部分）
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
