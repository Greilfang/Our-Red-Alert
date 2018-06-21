//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma warning (disable : 4996)
#define ASIO_STANDALONE
#ifndef CHAT_CLIENT
#define CHAT_CLIENT
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include<string>
using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
	//辅助函数
	bool error()const { return error_flag_; };
	void close() { do_close(); };
	bool started() const { return start_flag_; };
	int camp() const
	{
		while (!start_flag_);
		return camp_;
	}

	int total() const
	{
		while (!start_flag_);
		return total_;
	}
	static chat_client * create(std::string ip = "127.0.0.1", int port=1024)
	{
		auto s = new chat_client(ip, port);
		s->thread_=new std::thread(
		std::bind(static_cast<std::size_t (asio::io_context::*)()>(&asio::io_context::run),
			&s->io_context_));
			return s;
	}

	chat_client(std::string ip,int port): socket_(io_context_),
		endpoint_(asio::ip::address_v4::from_string(ip),port)
	{
		do_connect();
	}

	void write_data(std::string s)
	{
		if (error_flag_)
			return;
		chat_message msg;
		if (s.size() == 0)
		{
			s = std::string("\0");
			msg.body_length(1);
		}
		else
			msg.body_length(s.size());
		memcpy(msg.body(), &s[0u], msg.body_length());
		msg.encode_header();
		asio::write(socket_, asio::buffer(msg.data(), msg.length()));
		std::cout << "client write success\n";
	}
	std::string read_data()
	{
		if (error_flag_)
			return "";
		std::unique_lock<std::mutex> lk{ mut };
		while (read_msg_deque_.empty())
			data_cond_.wait(lk);

		auto read_msg = read_msg_deque_.front();
		read_msg_deque_.pop_front();
		lk.unlock();
		auto ret = std::string(read_msg.body(), read_msg.body_length());
		std::cout << "client read success\n";
		return ret;
	}
private:
	void do_connect()
	{
		socket_.async_connect(endpoint_,
			[this](const std::error_code &ec/*, tcp::endpoint*/)
		{
			try
			{
				if (!ec)
				{
					std::cout << "the connection has been constructed" << std::endl;
					
					char data[30] = { 0 };
					asio::error_code err;
					//同步读取,是阻塞的
					size_t length = socket_.read_some(asio::buffer(data, 30), err);
					std::cout << data<<std::endl;

					if (err || length < 10)	
						throw asio::system_error(err);
					/*解析第一次收到的指令,因为是同步的,保证了轮流接受*/
					char header[4 + 1] = "";
					strncat(header, data + 10, 4);
					total_ = atoi(header);
					camp_ = atoi(data + 14);
					start_flag_ = true;
					/*骚操作的结束*/
					do_read_header();
				}
				else
				{
					std::cerr << "failed to connect" << std::endl;
					error_flag_ = true;
				}
			}
			catch (std::exception & e)
			{
				std::cerr<<"exception in connection: " << e.what() << "\n";
			}
		});
	}

	void do_read_header()
	{
		async_read(socket_,
			asio::buffer(read_msg_.data(), chat_message::header_length),
			[this](std::error_code ec, std::size_t)
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();
			}
			else
			{
				do_close();
			}
		});
	}

	void do_read_body()
	{
		async_read(socket_,
			asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this](std::error_code ec, std::size_t)
		{
			if (!ec)
			{
				std::lock_guard<std::mutex>lk{ mut };
				read_msg_deque_.push_back(read_msg_);
				data_cond_.notify_one();
				std::cout << "read completed\n";
				//std::cout.write(read_msg_.body(), read_msg_.body_length());
				//std::cout << "\n";
				do_read_header();
			}
			else
			{
				do_close();
			}
		});
	}
	
	void do_close()
	{
		std::cout << "close!";
		try {
			std::lock_guard<std::mutex> lk{ mut };
			error_flag_ = true;
			chat_message empty_msg;
			memcpy(empty_msg.data(), "0001\0", 5);
			read_msg_deque_.push_back(empty_msg);

			std::cout << "before notify";
			data_cond_.notify_one();
			/****************疑似中断网络连接的************************/
			io_context_.stop();
			asio::error_code ec;
			socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
			/*
			if (!ec)
				throw asio::system_error(ec);
				*/
			socket_.close();
			thread_->join();

			std::cout << "close_try";
		}
		catch (std::exception&e)
		{

			e.what();
		}

	}
	/*
	void do_write()
	{
		asio::async_write(socket_,
			asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this](std::error_code ec)
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				socket_.close();
			}
		});
	}
	*/

private:
	asio::io_context io_context_;
	tcp::socket socket_;
	tcp::endpoint endpoint_;
	chat_message read_msg_;
	chat_message_queue read_msg_deque_;

	bool start_flag_{ false }, error_flag_{ false };

	std::thread *thread_, *read_thread_;

	int camp_, total_;
	
	std::condition_variable data_cond_;
	std::mutex mut;
};
#endif