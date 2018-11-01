#include <iostream>
#include <string>
#include <vector>
#include <cadef.h>
/*
    PVs to monitor
*/
std::string evid_pv = "CLA-L01-LRF-CTRL-01:ad1:dod_demod_vec.EVID";
std::string data_pv = "CLA-L01-LRF-CTRL-01:ad1:dod_demod_vec";
/*
    data is an array of length 24567
*/
size_t data_len = 24567;
std::vector<double> new_data(data_len, 0.0);
std::vector<double> old_data(data_len, 1.0);
/*
    evid is a string
*/
std::string old_evid = "old_evid";
std::string new_evid = "new_evid";
// we can also convert the EVID to a integer
int evid_int_0;
int evid_int;
/*
    counters
*/
size_t evid_entire_call = 0; // how many times the function has been called
size_t evid_unique_call = 0; // how many times the function is called with unique data
size_t data_entire_call = 0; // how many times the function has been called
size_t data_unique_call = 0; // how many times the function is called with unique data
/*
    Timestamps
*/
std::string data_time_0;
std::string evid_time_0;
std::string data_time;
std::string evid_time;
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

const dbr_time_string* p_evid;
const dbr_double_t   * p_evid_value;

void evid_callBack(const event_handler_args args)
{
    p_evid = (const struct dbr_time_string*)args.dbr;
    /*
        get time stamp
        Save firts time stamp adn print message
    */
    get_time(p_evid->stamp, evid_time);
    /*
        Get new EVID, if new EVID is same as last
    */
    new_evid = p_evid->value;

    if( new_evid == "" )
    {
        evid_int = -1;
    }
    else
    {
        evid_int = std::stoi(new_evid.c_str());
    }

    if( evid_entire_call == 0 )
    {
        evid_time_0 = evid_time;
        evid_int_0  = evid_int;
        std::cout << evid_time_0 << ", first EVID Timestamp, EVID =  " << evid_int_0 <<  std::endl;
        std::cout << std::endl;
    }


    if( new_evid != old_evid )
    {
        evid_unique_call += 1;
    }
    evid_entire_call += 1;


    if( evid_entire_call % 6000 == 0 )
    {

        std::cout << evid_time << " evid_entire_call = " << evid_entire_call
                  << ", evid_unique_call = " << evid_unique_call
                  << ", evid = " << evid_int - evid_int_0
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

    // chids for evid and data
    chid data_chid;
    chid evid_chid;
    // create channels
    ca_create_channel(evid_pv.c_str(), nullptr, nullptr, 0, &evid_chid);
    ca_create_channel(data_pv.c_str(), nullptr, nullptr, 0, &data_chid);
    // send to epics
    int status = ca_pend_io(5.0);
    status = ca_flush_io();
    SEVCHK(status, "ca_create_channel");

    evid data_evid;
    evid evid_evid;

    ca_create_subscription(DBR_TIME_DOUBLE, data_len, data_chid, DBE_VALUE, data_callBack, nullptr, &data_evid);
    ca_create_subscription(DBR_TIME_STRING, 1,        evid_chid, DBE_VALUE, evid_callBack, nullptr, &evid_evid);

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
