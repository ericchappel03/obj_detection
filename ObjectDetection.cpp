#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
void obj_detection(Mat frame);
CascadeClassifier face_cascade; 
CascadeClassifier eyes_cascade;

int main(int argc, char** argv) {
    std::string facepath = "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"; //pretrained model 
    std::string eyepath = "/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml"; 
    //open webcam
    VideoCapture capture;
    if(argc == 1){ // no arguments
    capture.open(0); //video0 is the right location for my video drivers
    capture.set(CAP_PROP_FRAME_WIDTH, 600);
    capture.set(CAP_PROP_FRAME_HEIGHT, 600);
    capture.set(CAP_PROP_FPS, 45);
    capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
    }
    else{
    capture.open(argv[1]); //specify video adapter 
    capture.set(CAP_PROP_FPS, 45);
    capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
    }
    //camera check     
    if(!capture.isOpened()){
        std::cout << "Error opening video stream" << std::endl;
        return -1;
    } 
    else{
        std::cout << "Webcam opened\n"; 
    }
    //read video 
     Mat frame;
        while(1){
       
        capture >> frame;
        if(frame.empty()){
            std::cout << "No captured frames\n";
            break;
        }
        face_cascade.load(facepath);
        eyes_cascade.load(eyepath);
        obj_detection(frame);
        
        //Display frame
        imshow("Frame", frame);
        //insert obj detection
        char c =(char)waitKey(25);
        if(c==27){ 
            break;//ASCII for ESC
        }
    }
    //release video capture object
    capture.release();
    destroyAllWindows();
    return 0; 
}

void obj_detection(Mat frame){
    //grayscale object 
    Mat frame_out;
    cvtColor(frame,frame_out,COLOR_BGR2GRAY);
    equalizeHist(frame_out,frame_out);
    std::vector<cv::Rect> faces; 
    face_cascade.detectMultiScale(frame_out, faces);
    for(size_t i = 0; i < faces.size(); i++){
        Point center(faces[i].x + faces[i].width/2, faces[i].y+ faces[i].height/2);
        ellipse(frame,center,Size(faces[i].width/2 ,faces[i].height/2),0,0,360,Scalar(255,0,255),4);
        Mat faceROI = frame_out(faces[i]);


        //eyes now 
        std::vector<Rect> eyes; 
        eyes_cascade.detectMultiScale(faceROI, eyes);
        for(size_t j = 0; j < eyes.size(); j++){
        Point eyes_center(faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height/2);
        int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
        circle(frame, eyes_center, radius, Scalar(255,0,0),4);
    }
    }
}