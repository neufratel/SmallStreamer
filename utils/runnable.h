#ifndef RSO_RUNNABLE_H
#define RSO_RUNNABLE_H


#include <thread>
#include <iostream>



class Runnable{
public:
    void start(){
        if(run_==false){
            run_=true;
            t_run = std::thread(&Runnable::run, this);
        }else{
            std::cout<<"Object already is runing main thread"<<std::endl;
        }
    }
    void start_sec(){
	if(run_sec_==false){
		run_sec_=true;
		t_run_sec = std::thread(&Runnable::run_sec, this);		
	}else{
		std::cout<<"Object already is ruing secondary thread"<<std::endl;
	}
    }
	
    void stop(){run_=false; };
    void stop_sec(){run_sec_=false;};
    explicit Runnable():run_(false), run_sec_(false){ run_=false;	}
    ~Runnable() {};
protected:
    virtual void run()=0;
    virtual void run_sec(){};
    volatile bool run_;
    volatile bool run_sec_;
private:
    std::thread t_run; 
    std::thread t_run_sec;
};

#endif
