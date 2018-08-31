/*
//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      DJS
//  Last edit:   08-06-2018
//  FileName:    runningStat.h
//  Description: thanks to https://www.johndcook.com/blog/standard_deviation/
//               keeps running stats of mean, variance for numbers Push(ed)
//
//*/
#ifndef _VC_RUNNING_STAT_H_
#define _VC_RUNNING_STAT_H_

#include <iostream>

class runningStat
{
    // thanks to https://www.johndcook.com/blog/standard_deviation/

    public:
        runningStat() :
            m_n(0) ,
            max_n(0),
            rs_complete(false)
            {}

        void Clear()
        {
            m_n = 0;
            rs_complete = false;
        }

        void setMaxCount(const size_t value)
        {
            max_n = value;
        }

        size_t getMaxCount() const
        {
            return max_n;
        }

        void Push(double x)
        {
            if(can_add())
            {
                m_n++;
                // See Knuth TAOCP vol 2, 3rd edition, page 232
                if (m_n == 1)
                {
                    m_oldM = m_newM = x;
                    m_oldS = 0.0;
                }
                else
                {
                    m_newM = m_oldM + (x - m_oldM)/m_n;
                    m_newS = m_oldS + (x - m_oldM)*(x - m_newM);

                    // set up for next iteration
                    m_oldM = m_newM;
                    m_oldS = m_newS;
                }
            }
        }

        size_t NumDataValues() const
        {
            return m_n;
        }

        double Mean() const
        {
            return (m_n> 0) ? m_newM : 0.0;
        }

        double Variance() const
        {
            return ( (m_n> 1) ? m_newS/(m_n - 1) : 0.0 );
        }

        double StandardDeviation() const
        {
            return sqrt( Variance() );
        }

        bool Full() const
        {
            return rs_complete;
        }

        bool NotFull() const
        {
            return !Full();
        }


    private:
        size_t m_n;
        double m_oldM, m_newM, m_oldS, m_newS;
        size_t max_n;
        bool rs_complete;

        bool can_add()
        {
            if(max_n == 0)
            {
                return true;
            }
            if(rs_complete)
            {
                return false;
            }

            if(m_n > max_n)
            {
                rs_complete = true;
                return false;
            }
            return true;
        }

};
#endif // _VC_RUNNING_STAT_H_
