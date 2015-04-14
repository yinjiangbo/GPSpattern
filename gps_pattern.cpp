#include <functional>
#include <limits>
#include <map>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "gps_pattern.hpp"
#include <algorithm>
#include <fstream>
#include "boost/lexical_cast.hpp"

ifstream& openFile(ifstream& in, const string& fileName)
{
    in.close();
    in.clear();
    in.open(fileName.c_str());
    return in;
}

vector<string> split(const string& s, const string& delim, const bool keep_empty = true) {
    vector<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

gps_pattern::gps_pattern(){}

 tm * gps_pattern::convertTime(long t){
    time_t timep=t;
    return localtime(&timep);
}

week_data gps_pattern::convert2weekdata(long t, double vel){
    week_data data;
    tm *p=convertTime(t);
    if(p->tm_min>=30)
        p=convertTime(t+1800);
    data.year=1900 + p->tm_year;
    data.month=1 + p->tm_mon;
    data.day=p->tm_mday;
    data.week=p->tm_wday;
    data.hour=p->tm_hour;
    data.vel=vel;
    return data;
}
vector<week_data> gps_pattern::loadGPS(const string& filename)
{
    vector<week_data> Temp_GPS_point;
    long t;
    double vel;
    ifstream fread;
    if (openFile(fread, filename)){
        while (fread >> t >> vel)
            if(vel>1&&vel<45)
                Temp_GPS_point.push_back(convert2weekdata(t,vel));
    }
    else{
        cout << "Load file failed!" << endl;
    }
    return  Temp_GPS_point;
}

void gps_pattern::getmedian(vector<week_data> weekdatas, double  arrays[][24]){

 //   for(int i=0;i<7;++i)
  //      for(int j=0;j<24;++j)
 //           arrays[i][j]=0;
    if(weekdatas.size()<7*24)
        return ;
    vector<double> vels[7][24] ;
    for(int i=0;i<weekdatas.size();++i)
        vels[weekdatas[i].week][weekdatas[i].hour].push_back(weekdatas[i].vel);
    for(int i=0;i<7;++i){
        for(int j=0;j<24;++j){
            if(vels[i][j].size()>0){
                std::sort (vels[i][j].begin(), vels[i][j].end());
                double sum=0;
                int start=0;
                int end=vels[i][j].size()-1;
                if(vels[i][j].size()>10){
                    start=vels[i][j].size()/4;
                    end=vels[i][j].size()-1-vels[i][j].size()/4;
                }
                for(int k=start;k<=end;++k)
                        sum+=vels[i][j][k];
                arrays[i][j]=sum/(end-start+1);
            }
        }
    }
    for(int i=0;i<7;++i){
        if(arrays[i][0]<1)
            arrays[i][0]=arrays[i][1];
        if(arrays[i][23]<1)
            arrays[i][23]=arrays[i][22];
        for(int j=1;j<23;++j)
            if(arrays[i][j]<1)
                arrays[i][j]=max(arrays[i][j-1],arrays[i][j+1]);
    }

}

void gps_pattern::write(){
	DIR *pDir;
	struct dirent* ent=NULL;
    char dirname[]="./Data/routes/";
    string dir="./Data/routes/";
    vector<string> filenames;
    vector<string> relativeFilenames;
   	if((pDir=opendir(dirname))==NULL)
        cout<<"Not Found"<<endl;
	while(NULL!=(ent=readdir(pDir)))
    {
		if(strcmp(ent->d_name,".")==0||strcmp(ent->d_name,"..")==0)
            continue;
        filenames.push_back(ent->d_name);
    }
    /*
    for(int i=0;i<filenames.size();++i){
            string oldname=dir+filenames[i];
            string newname=oldname;
            size_t pos = filenames[i].find("routes");
            if(pos>=0)
                newname=dir+filenames[i].substr(6);
            if (rename(oldname.c_str(), newname.c_str()) == 0)
                printf("Renamed %s to %s./n", oldname.c_str(), newname.c_str());
            else
                perror("rename");
        }

    for(int i=0;i<7;++i){
        for(int j=0;j<24;++j){
            FILE *file_A;
            string str=dir2+boost::lexical_cast<string>(i)+"_"+boost::lexical_cast<string>(j);
            file_A = fopen( str.c_str(), "w+" );
            fclose(file_A);
        }
    }

    string dir2="./Data/timeInterval/";

    vector<route_meanVel> meanVels[7][24] ;
    double arrays[7][24] ={0.0};
    for(int i=0;i<filenames.size();++i){
        vector<week_data> weekdatas=loadGPS(dir+filenames[i]);
        getmedian(weekdatas,arrays );
        for(int j=0;j<7;++j){
            for(int k=0;k<24;++k){
                if(arrays[j][k]>0){
                   route_meanVel meanvel;
                   meanvel.routeid=filenames[i];
                   meanvel.vel=arrays[j][k];
                   meanVels[j][k].push_back(meanvel);
                }
            }
        }
    }

    for(int i=0;i<7;++i){
        for(int j=0;j<24;++j){
            FILE *file_A;
            string str=dir2+boost::lexical_cast<string>(i)+"_"+boost::lexical_cast<string>(j);
            file_A = fopen( str.c_str(), "w+" );
            for(int k=0;k<meanVels[i][j].size();++k){
                fprintf(file_A, "%s %f\n",meanVels[i][j][k].routeid.c_str(),meanVels[i][j][k].vel);
            }
            fclose(file_A);
        }
    }
    */
    string dir3="./Data/Interval/";
    for(int i=0;i<filenames.size();++i){
        vector<week_data> weekdatas=loadGPS(dir+filenames[i]);
        FILE *file_A;
        string str=dir3+filenames[i];
        file_A = fopen( str.c_str(), "w+" );
        for(int k=0;k<weekdatas.size();++k){
            fprintf(file_A, "%d %d %f\n",weekdatas[k].week,weekdatas[k].hour,weekdatas[k].vel);
        }
        fclose(file_A);
    }
}

void gps_pattern::generateXML(){
    string dir1="./Data/timeInterval/";
    string dir2="./Data/velocity_xmls/";
    for(int i=0;i<7;++i){
        for(int j=0;j<24;++j){
            FILE *file_A;
            string str=dir1+boost::lexical_cast<string>(i)+"_"+boost::lexical_cast<string>(j);
            string newstr=dir2+boost::lexical_cast<string>(i)+"_"+boost::lexical_cast<string>(j)+".xml";
            file_A = fopen( newstr.c_str(), "w+" );
            fprintf(file_A, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            fprintf(file_A, "<meandata xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://sumo-sim.org/xsd/meandata_file.xsd\">\n");
            fprintf(file_A, "<interval begin=\"0.00\" end=\"6000.00\" id=\"lane_stats\">\n");
            ifstream fread;
            string routeid;
            double vel;
            if (openFile(fread, str)){
                while (fread >> routeid >> vel)
                   fprintf(file_A, "<edge id=\"%s\">\n<lane id=\"%s_0\" sampledSeconds=\"0.0\" traveltime=\"0.0\" density=\"0.0\" occupancy=\"0.0\" waitingTime=\"0.00\" speed=\"%f\" departed=\"0\" arrived=\"0\" entered=\"0\" left=\"0\" laneChangedFrom=\"0\" laneChangedTo=\"0\"/>\n</edge>\n",routeid.c_str(),routeid.c_str(),vel);
            }
            else{
                cout << "Load file failed!" << endl;
            }
            fprintf(file_A, "</interval>\n");
            fprintf(file_A, "</meandata>\n");
            fclose(file_A);
        }
    }

}



