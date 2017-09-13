#include <stdio.h>
#include <sstream>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<vector>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char** argv)
{
    argc = 2;
    char pathName[10000] =
            "/mnt/0E0616A706169037/work/DataSet/TUM/rgbd_dataset_freiburg3_long_office_household";
    argv[1] = pathName;

    if(argc != 2)
    {
        cout << "error: Please enter a path name!" << endl;
        return -1;
    }
    if( opendir(argv[1]) == NULL )
    {
        cout << "error: Please check the path" << endl;
    }

    string path(argv[1]);
    if( path[path.size()-1] != '/' ) path.push_back('/');

    if( opendir((path+"imu0").c_str()) == NULL )
        cout << "error: please generate the IMU data!" << endl;

    string pathRGB = path + "rgb/";
    string pathCAM = path + "cam0/";
    string pathCAMdata = path + "cam0/data/";

    bool euroc_flag = true;
    bool tum_flag = true;

    DIR *dpE, *dpT;
    if( (dpT = opendir(pathRGB.c_str())) == NULL )
        tum_flag = false;
    if( (dpE = opendir(pathCAMdata.c_str())) == NULL )
        euroc_flag = false;

    if( !euroc_flag && !tum_flag )
    {
        cout << "error: Can not find the correct folder for the images!" << endl;
        return -1;
    }

    bool euroc_flag1 = euroc_flag;
    bool tum_flag1 = tum_flag;

    struct dirent *dirpE, *dirpT;
    vector<string> fileNameE, fileNameT;

    if(euroc_flag)
        while((dirpE = readdir(dpE)) != NULL)
        {
            string t_name(dirpE->d_name);
            if(t_name[t_name.size()-1] != 'g') continue;

            int dot_num = 0;
            for(int i = 0; i<t_name.size(); i++)
            {
                if(t_name[i] == '.') dot_num++;
            }
            if(dot_num == 1)
                fileNameE.push_back(t_name);
            else
                euroc_flag1 = false;
        }


    if(tum_flag)
        while((dirpT = readdir(dpT)) != NULL)
        {
            string t_name(dirpT->d_name);
            if(t_name[t_name.size()-1] != 'g') continue;

            int dot_num = 0;
            for(int i = 0; i<t_name.size(); i++)
            {
                if(t_name[i] == '.') dot_num++;
            }
            if(dot_num == 2)
                fileNameT.push_back(t_name);
            else
                tum_flag1 = false;
        }

    if( !tum_flag1 && !euroc_flag1 )
    {
        cout << "error: Bad folder for images!" << endl;
        return -1;
    }
    if( tum_flag1 && euroc_flag1 )
        return 0;

    if(tum_flag1)
    {
        mkdir((pathRGB + "data/").c_str(), S_IRWXO);
        for(int i = 0; i<fileNameT.size(); i++ )
        {
            string &fnO = fileNameT[i];
            double fn = atof(fnO.substr(0,fnO.size()-4).c_str())*1e9;
            char ss[1000];
            sprintf(ss,"%.0f", fn);
            string fnN(ss);
            fnN += ".png";

            rename((pathRGB + fnO).c_str(), (pathRGB + "data/" + fnN).c_str());
        }
        rename(pathRGB.c_str() , pathCAM.c_str());
    }
    else
    {
        for(int i = 0; i<fileNameE.size(); i++ )
        {
            string &fnO = fileNameE[i];
            double fn = atof(fnO.substr(0,fnO.size()-4).c_str())/1e9;
            char ss[1000];
            sprintf(ss,"%.6f", fn);
            string fnN(ss);
            fnN += ".png";

            rename((pathCAMdata + fnO).c_str(), (pathCAM + fnN).c_str());
        }
        remove(pathCAMdata.c_str());
        rename(pathCAM.c_str() , pathRGB.c_str());

    }


    return 0;

}
