#if 0
  #include <sys/time.h>
  class MyTimer  //time used
  {
   public:
    MyTimer() { gettimeofday( &_start_time, NULL ); }
    double elapsed() const        
    { 
       struct timeval end;
       gettimeofday( &end, NULL );
       float time_use=(end.tv_sec-_start_time.tv_sec)*1000+(end.tv_usec-_start_time.tv_usec)/1000;//毫秒
       
      return  time_use/1000; 
    }

    private:
      struct timeval _start_time;
  }; // MyTimer
#else
  class MyTimer
  {
   public:
    MyTimer() {  }
    void   restart() { return; }
    double elapsed() const        
    { return 0; }

    double elapsed_max() const  
    {
      return 0; 
    }

    double elapsed_min() const         
    { return 0; }
  }; // MyTimer
#endif

