/***************************************************************************
  tag: Peter Soetens  Wed Jan 18 14:09:48 CET 2006  ControlTaskI.h

                        ControlTaskI.h -  description
                           -------------------
    begin                : Wed January 18 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/


// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:844

#ifndef CONTROLTASKI_H_
#define CONTROLTASKI_H_

#include "../rtt-config.h"
#include "corba.h"
#ifdef CORBA_IS_TAO
#include "ControlTaskS.h"
#else
#include "ControlTaskC.h"
#endif

#include "OperationInterfaceC.h"
#include "AttributesC.h"
#include "ScriptingAccessC.h"
#include "ServicesC.h"
#include "DataFlowC.h"
#include "../../TaskContext.hpp"
#include "../../interface/OperationInterface.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Orocos_CAttributeInterface_i;
class Orocos_CMethodInterface_i;
class Orocos_CCommandInterface_i;

class  Orocos_CControlObject_i
    : public virtual POA_RTT::corba::CControlObject
    , public virtual PortableServer::RefCountServantBase
{
protected:
    PortableServer::POA_var mpoa;

private:
    RTT::interface::OperationInterface* mobj;

    RTT::corba::CAttributeInterface_var mAttrs;
    RTT::corba::CMethodInterface_var    mMFact;
    RTT::corba::CCommandInterface_var   mCFact;

    PortableServer::ServantBase_var mAttrs_i;
    PortableServer::ServantBase_var mMFact_i;
    PortableServer::ServantBase_var mCFact_i;

    typedef std::map<std::string, Orocos_CControlObject_i*> CTObjMap;
    CTObjMap ctobjmap;

public:
  //Constructor
  Orocos_CControlObject_i (RTT::interface::OperationInterface* orig, PortableServer::POA_ptr the_poa);

  //Destructor
  virtual ~Orocos_CControlObject_i (void);

    PortableServer::POA_ptr _default_POA();

  virtual
  char * getName (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  char * getDescription (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CAttributeInterface_ptr attributes (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CMethodInterface_ptr methods (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CCommandInterface_ptr commands (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CObjectList* getObjectList()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CControlObject_ptr getObject(const char*)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean hasObject(const char*)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

};

class  Orocos_CControlTask_i
: public virtual POA_RTT::corba::CControlTask, public virtual PortableServer::RefCountServantBase,
  public Orocos_CControlObject_i
{
protected:
    RTT::TaskContext* mtask;

    RTT::corba::CScriptingAccess_var mEEFact;
    RTT::corba::CServiceInterface_var mService;
    RTT::corba::CDataFlowInterface_var mDataFlow;

    PortableServer::ServantBase_var mEEFact_i;
    PortableServer::ServantBase_var mService_i;
    PortableServer::ServantBase_var mDataFlow_i;

public:
  //Constructor
  Orocos_CControlTask_i (RTT::TaskContext* orig, PortableServer::POA_ptr the_poa);

  //Destructor
  virtual ~Orocos_CControlTask_i (void);

  void shutdownCORBA();

  virtual
  ::RTT::corba::CTaskState getTaskState (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean start (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean stop (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean isActive (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean isRunning (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean configure (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean cleanup (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean resetError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean activate (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean isConfigured (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean inFatalError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

    virtual
  CORBA::Boolean inRunTimeWarning (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

    virtual
  CORBA::Boolean inRunTimeError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

    virtual
  CORBA::Long getWarningCount (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

    virtual
  CORBA::Long getErrorCount (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));


  virtual
  ::RTT::corba::CScriptingAccess_ptr scripting (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CServiceInterface_ptr services (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CDataFlowInterface_ptr ports (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));


  virtual
  ::RTT::corba::CControlTask::CControlTaskNames * getPeerList (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  ::RTT::corba::CControlTask_ptr getPeer (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean addPeer (
      ::RTT::corba::CControlTask_ptr p,
      const char * alias
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean hasPeer (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean removePeer (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean connectPeers (
      ::RTT::corba::CControlTask_ptr p
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean disconnectPeers (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean connectPorts (
      ::RTT::corba::CControlTask_ptr p
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

};


#endif /* CONTROLTASKI_H_  */
