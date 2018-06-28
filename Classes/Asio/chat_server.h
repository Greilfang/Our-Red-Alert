// chat_server.cpp
// ~~~~~~~~~~~~~~~
//借鉴了asio库的官网demo
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
#include "GameMessageWrap.h"
using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------
class chat_server;

/*
*   @brief  tcp端口管理类
*           负责储存客户端端口号, 同步写消息读消息
*/
class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	
	/**
	* @brief                        从服务端返回一个客户端指针
	*
	* @param io_service             给客户端分配一个任务队列
	* @param parent                 返回的客户端来自服务端对象
	*/
	static pointer create(asio::io_service& io_service, chat_server* parent);
	
	/**
	* @brief                        给出端口使用权
	*/
	tcp::socket& socket();

	/**
	* @brief                        从服务端得到的套接字
	*
	* @param socket                 把端口号从server转给tcpconnection
	*/
	void get_socket(tcp::socket);

	/**
	* @brief                        开始读取内容
	*/
	void start();
	
	/**
	* @brief                        写内容给服务端
	*/
	void write_data(std::string s);

	/**
	* @brief                        读取内容的具体操作
	*
	*/
	std::string read_data();

	/**
	* @brief                        确定有没有出错
	*
	*/
	bool error()const { return error_flag_; }

	/**
	* @brief                        关闭进程和端口号
	*
	*/
	void do_close();
private:
	
	/**
	* @brief                        处理消息正文内容
	*
	*/
	void handle_read_header(const asio::error_code& error);
	
	/**
	* @brief                        处理消息正文内容
	*
	*/
	void handle_read_body(const asio::error_code& error);

	/**
	* @brief                        创建一个tcp连接
	*
	* @param io_service             给客户端分配一个任务队列
	* @param parent                 返回的客户端来自服务端对象
	*/
	TcpConnection(asio::io_service& io_service, chat_server * parent);;

	tcp::socket socket_;             //把建立连接的套接字记录下来

	chat_server* parent;             //服务端指针

	bool error_flag_{ false };       //错误标志

	chat_message read_msg_;          //当前读取消息
	std::deque<chat_message> read_msg_deque_; //读取消息要存入待解析队列
	std::condition_variable data_cond_;  //条件变量
	std::mutex mut_;                 //进程锁
	//	asio::steady_timer steady_timer_;

};
/**
* @brief                        服务端
*
*/
class chat_server
{
public:
	/**
	* @brief                    地图号
	*
	*/
	int map = 2;
	
	/**
	* @brief                        给接收器一个任务队列
	*
	* @param io_service             给服务端分配一个任务队列
	* @param endpoint               接受的端口号
	*/
	chat_server(int port) :
		acceptor_(*io_service_, tcp::endpoint(tcp::v4(), port))
	{
		do_accept();
	}

	/**
	* @brief                      返回一个服务端指针
	*
	* @param port                 读取的端口号
	*/
	static chat_server *create(int port = 1024) {
		auto s = new chat_server(port);
		s->thread_ = new std::thread(
			std::bind(static_cast<std::size_t(asio::io_service::*)()>(&asio::io_service::run),
				io_service_));
	
		return s;
	}
	
	/**
	* @brief                        发送给客户端的玩家初始信息
	*
	*/
	void button_start()
	{
		acceptor_.close();
		using namespace std; // For sprintf and memcpy.
		char total[4 + 1] = "";
		sprintf(total, "%4d", static_cast<int>(connections_.size()));
		char camp[4 + 1] = "";
		for (auto i = 0; i < connections_.size(); i++) {
			sprintf(camp, "%4d", i + 1);
			connections_[i]->write_data("PLAYER" + std::string(total) + std::string(camp) + std::to_string(map));
		}
		this->button_thread_ = new std::thread(std::bind(&chat_server::loop_process, this));
		button_thread_->detach();
	}
	int connection_num() const
	{
		return connections_.size();
	}
private:
	/**
	* @brief                        异步尝试接受来自客户端的接受请求
	*
	*/
	void do_accept()
	{
		acceptor_.async_accept(
			[this](std::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				TcpConnection::pointer new_connection = TcpConnection::create(acceptor_.get_io_context(), this);
				connections_.push_back(new_connection);
				auto ep_ = socket.remote_endpoint();
				new_connection->get_socket(std::move(socket));
				std::cout << "client : " << ep_.port() << " enter this room" << std::endl;
				new_connection->start();
				//std::make_shared<TcpConnection>(std::move(socket))->start();
			}
			do_accept();
		});
	}
	
	/**
	* @brief                        给客户端同步发送信息
	*
	*/
	void loop_process()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(delete_mutex_);
			std::vector<std::string> ret;
			for (auto r : connections_)
			{
				if (r->error())
					//				break;
					error_flag_ |= r->error();
				ret.push_back(r->read_data());
			}
			auto game_msg = GameMessageWrap::combine_message(ret);

			for (auto r : connections_)
				r->write_data(game_msg);
		}
	}
	std::vector<TcpConnection::pointer> connections_;           //存放tcp连接的容器
	static asio::io_service * io_service_;                      //给服务端的一个
	tcp::acceptor acceptor_;                                    //接受器
	std::thread *thread_, *button_thread_{ nullptr };           //开始游戏信息写入的进程
	std::mutex delete_mutex_;                                   //锁
	bool error_flag_{ false };                                  //错误标志
	std::condition_variable data_cond_;                         //条件变量
	//chat_room room_;
};
#endif
//----------------------------------------------------------------------
