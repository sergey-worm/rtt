/***************************************************************************
  tag: Peter Soetens  Mon May 10 19:10:29 CEST 2004  main.cxx 

                        main.cxx -  description
                           -------------------
    begin                : Mon May 10 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

/**
 * @file main.cxx
 * This file contains the start-up code of the framework
 * and will call in turn the user's ORO_main() function.
 */

#include <os/startstop.h>
#include <os/main.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    __os_init(argc, argv);

    int res;
    try {
        res = ORO_main(argc, argv);
    }
    catch( ... )
        {
            cout <<endl<< " Orocos has detected an uncaught C++ exception"<<endl;
            cout << " in the ORO_main() function."<<endl;
            cout << " You might have called a function which throws"<<endl;
            cout << " without a try {} catch {} block."<< endl << endl;
        }

    __os_exit();

    return res;
}
