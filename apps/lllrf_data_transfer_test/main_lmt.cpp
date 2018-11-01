#include <iostream>
#include <string>
#include <vector>
#include <cadef.h>
/*
    PVs to monitor
*/
std::string lmt__pv = "CLA-L01-LRF-CTRL-01:ad1:dod_demod_vec.LMT";
std::string data_pv = "CLA-L01-LRF-CTRL-01:ad1:dod_demod_vec";
/*
    data is an array of length 24567
*/
size_t data_len = 24567;
std::vector<double> new_data(data_len, 0.0);
std::vector<double> old_data(data_len, 1.0);
/*
    lmt is a string
*/
std::string old_lmt = "old_lmt";
std::string new_lmt = "new_lmt";
// we can also convert the LMT to a integer
int lmt_int_0;
int lmt_int;
/*
    counters
*/
size_t lmt__entire_call = 0; // how many times the function has been called
size_t lmt__unique_call = 0; // how many times the function is called with unique data
size_t data_entire_call = 0; // how many times the function has been called
size_t data_unique_call = 0; // how many times the function is called with unique data
/*
    Timestamps
*/
std::string data_time_0;
std::string lmt__time_0;
std::string data_time;
std::string lmt__time;



/*
    Return the time stamp as a string
    for hints look in epicsTime.h
*/
void get_time(const epicsTimeStamp& stamp, std::string& str)
{
    char timeString[36];
    epicsTimeToStrftime(timeString, sizeof(timeString),"%a %b %d %Y %H:%M:%S.%f", &stamp);
    str = timeString;
}
/*
    Return the time stamp as a string
    for hints look in epicsTime.h
*/
const dbr_time_string* p_lmt;
const dbr_double_t   * p_lmt_value;
void evid_callBack(const event_handler_args args)
{
    p_lmt = (const struct dbr_time_string*)args.dbr;
    /*
        get time stamp
        Save firts time stamp adn print message
    */
    get_time(p_lmt->stamp, lmt_time);
    /*
        Get new LMT, if new LMT is same as last
    */
    new_lmt = p_lmt->value;

    if( new_lmt == "" )
    {
        lmt_int = -1;
    }
    else
    {
        lmt_int = std::stoi(new_lmt.c_str());
    }

    if( lmt_entire_call == 0 )
    {
        lmt_time_0 = lmt_time;
        lmt_int_0  = lmt_int;
        std::cout << lmt_time_0 << ", first LMT Timestamp, LMT =  " << lmt_int_0 <<  std::endl;
        std::cout << std::endl;
    }


    if( new_lmt != old_lmt )
    {
        lmt_unique_call += 1;
    }
    lmt_entire_call += 1;


    if( lmt_entire_call % 6000 == 0 )
    {

        std::cout << lmt_time << " lmt_entire_call = " << lmt_entire_call
                  << ", lmt_unique_call = " << lmt_unique_call
                  << ", lmt = " << lmt_int - lmt_int_0
                  << std::endl;
    }
}
const dbr_time_double* p_data;
const dbr_double_t   * p_data_value;
void data_callBack(const event_handler_args args)
{

    p_data = (const struct dbr_time_double*)args.dbr;

    get_time(p_data->stamp,data_time);


    p_data_value = &p_data->value;
    std::copy(p_data_value, p_data_value + new_data.size(), new_data.begin() );


    if( data_entire_call == 0 )
    {
        data_time_0 = data_time;
        std::cout << data_time_0 << ", First Data Timestamp " << std::endl;
        std::cout << std::endl;
    }
    data_entire_call += 1;

    if( new_data != old_data )
    {
        data_unique_call += 1;
    }
    if( data_entire_call % 6000 == 0 )
    {
        std::cout << data_time << " data_entire_call = "   << data_entire_call
                  << ", data_unique_call = " << data_unique_call
                  << std::endl;
    }
}


int main()
{
    //
    ca_context_create(ca_enable_preemptive_callback);

    // chids for lmt and data
    chid data_chid;
    chid lmt_chid;
    // create channels
    ca_create_channel(lmt_pv.c_str(), nullptr, nullptr, 0, &lmt_chid);
    ca_create_channel(data_pv.c_str(), nullptr, nullptr, 0, &data_chid);
    // send to epics
    int status = ca_pend_io(5.0);
    status = ca_flush_io();
    SEVCHK(status, "ca_create_channel");

    lmt data_lmt;
    lmt lmt_lmt;

    ca_create_subscription(DBR_TIME_DOUBLE, data_len, data_chid, DBE_VALUE, data_callBack, nullptr, &data_lmt);
    ca_create_subscription(DBR_TIME_STRING, 1,        lmt_chid, DBE_VALUE, lmt_callBack, nullptr, &lmt_lmt);

    status = ca_pend_io(5.0);
    status = ca_flush_io();
    SEVCHK(status, "ca_create_subscription");

    if(status == ECA_NORMAL)
    {
        std::cout << "Successfully Registered Callbacks " << std::endl;
    }

    std::cout << "Running" << std::endl;
    std::cout << std::endl;
    while(1)
    {

    }
    return 0;
}
