/*
 *  anytun
 *
 *  The secure anycast tunneling protocol (satp) defines a protocol used
 *  for communication between any combination of unicast and anycast
 *  tunnel endpoints.  It has less protocol overhead than IPSec in Tunnel
 *  mode and allows tunneling of every ETHER TYPE protocol (e.g.
 *  ethernet, ip, arp ...). satp directly includes cryptography and
 *  message authentication based on the methodes used by SRTP.  It is
 *  intended to deliver a generic, scaleable and secure solution for
 *  tunneling and relaying of packets of any protocol.
 *
 *
 *  Copyright (C) 2007-2009 Othmar Gsenger, Erwin Nindl,
 *                          Christian Pointner <satp@wirdorange.org>
 *
 *  This file is part of Anytun.
 *
 *  Anytun is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  Anytun is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with anytun.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ANYTUN_syncTcpConnection_h_INCLUDED
#define ANYTUN_syncTcpConnection_h_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#include <string>

class SyncTcpConnection
  : public boost::enable_shared_from_this<SyncTcpConnection>
{
public:
  typedef boost::shared_ptr<SyncTcpConnection> pointer;
  typedef boost::asio::ip::tcp proto;

  static pointer create(boost::asio::io_service& io_service) {
    return pointer(new SyncTcpConnection(io_service));
  };

  boost::function<void(SyncTcpConnection*)> onConnect;
  proto::socket& socket();

  void start();
  void Send(std::string message);
private:
  SyncTcpConnection(boost::asio::io_service& io_service);

  void handle_write(const boost::system::error_code & /*error*/,
                    size_t /*bytes_transferred*/);
  proto::socket socket_;
};

#endif
