// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma warning (disable : 4996)
#define ASIO_STANDALONE
#ifndef CHAT_SERVER
#define CHAT_SERVER
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------
class chat_server;
class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	//	~TcpConnection();
	static pointer create(asio::io_service& io_service, chat_server* parent);
	tcp::socket& socket();
	
	void get_socket(tcp::socket);
	
	void start();

	void write_data(std::string s);

	std::string read_data();
	bool error()const { return error_flag_; }


	void do_close();
private:

	void handle_read_header(const asio::error_code& error);

	void handle_read_body(const asio::error_code& error);
	
	TcpConnection(asio::io_service& io_service, chat_server * parent);;

	//void check_timer();
	//void delete_from_parent();

	
	tcp::socket socket_;//把建立连接的套接字记录下来
	
	chat_server* parent;

	bool error_flag_{ false };

	chat_message read_msg_;
	std::deque<chat_message> read_msg_deque_;
	std::condition_variable data_cond_;
	std::mutex mut_;
	//	asio::steady_timer steady_timer_;

};

class chat_server
{
public:
	chat_server(int port) :
		acceptor_(*io_service_, tcp::endpoint(tcp::v4(), port)) 
	{
		do_accept();
	}

	static chat_server *create(int port = 1024) {
		auto s = new chat_server(port);
		s->thread_ = new std::thread(
			std::bind(static_cast<std::size_t(asio::io_service::*)()>(&asio::io_service::run),
				io_service_));
		return s;
	}

	void button_start()
	{
		acceptor_.close();
		using namespace std; // For sprintf and memcpy.
		char total[4 + 1] = "";//+1是给空字符
		sprintf(total, "%4d", static_cast<int>(connections_.size()));

		for (auto i = 0; i < connections_.size(); i++)
		{
			connections_[i]->write_data("PLAYER" + std::string(total) + std::to_string(i + 1));
			//cout<<"write content:"<<connections_[i]->read
		}

		connection_num_ = connections_.size();
		this->button_thread_ = new std::thread(std::bind(&chat_server::loop_process, this));
		button_thread_->detach();
	}

	void loop_process()
	{
		while (true)
		{
			if (connections_.size() != connection_num_)
			{
				error_flag_ = true;
				break;
			}
			std::unique_lock<std::mutex> lock(delete_mutex_);
			std::vector<std::string> ret;
			for (auto r : connections_)
			{
				if (r->error())
					//				break;
					error_flag_ |= r->error();
				ret.push_back(r->read_data());
			}
			//包装消息,可以先不用
			//auto game_msg = GameMessageWrap::combine_message(ret);
			/*
			for (auto r : connections_)
				r->write_data(game_msg);
				*/
		}
	}


private:
	void do_accept()
	{
		acceptor_.async_accept(
			[this](std::error_code ec, tcp::socket socket)
			//一次拷贝构造函数,所以调用了两次
		{
			if (!ec)
			{
				TcpConnection::pointer new_connection = TcpConnection::create(acceptor_.get_io_context(), this);
				//std::cout << "after definition" << new_connection.use_count() << std::endl;
				connections_.push_back(new_connection);
				//std::cout << "after push_back" << new_connection.use_count() << std::endl;
				auto ep_ = socket.remote_endpoint();
				new_connection->get_socket(std::move(socket));
				std::cout << "client : " << ep_.port() << " enter this room" << std::endl;
				new_connection->start();
				//std::make_shared<TcpConnection>(std::move(socket))->start();
			}
			do_accept();
		});

	}
	std::vector<TcpConnection::pointer> connections_;
	static asio::io_service * io_service_;
	tcp::acceptor acceptor_;
	std::thread *thread_, *button_thread_{ nullptr };
	//目前表示的连接数
	int connection_num_;
	/*锁和错误标识符*/
	std::mutex delete_mutex_;
	bool error_flag_{ false };
	std::condition_variable data_cond_;
};
#endif
//----------------------------------------------------------------------
