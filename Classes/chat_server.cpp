#pragma warning (disable : 4996)
#define ASIO_STANDALONE
#include"chat_server.h"
#include"asio.hpp"
#include<iostream>
asio::io_service* chat_server::io_service_ = new asio::io_service;

//TcpConnection::~TcpConnection()
//{
//	std::cout << "delete";
//	delete_from_parent();
//}

TcpConnection::pointer TcpConnection::create(asio::io_service& io_service, chat_server * parent)
{
	//返回一个引用计数的指针
	return pointer(new TcpConnection(io_service, parent));
}

void TcpConnection::get_socket(tcp::socket sock) {
	socket_ = std::move(sock);
}

tcp::socket& TcpConnection::socket()
{
	return socket_;
}

void TcpConnection::start()
{
	//不断读取数据
	asio::async_read(socket_,
		asio::buffer(read_msg_.data(), chat_message::header_length),
		std::bind(&TcpConnection::handle_read_header, this,
			std::placeholders::_1));
}

void TcpConnection::write_data(std::string s)
{
	if (error_flag_) return;
	chat_message msg;
	if (s.size() == 0)
	{
		s = std::string("\0");
		msg.body_length(1);
	}
	else
	{
		msg.body_length(s.size());
	}
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encode_header();

	std::cout << "example :" << msg.data() << std::endl;

	asio::write(socket_,
		asio::buffer(msg.data(), msg.length()));
}

std::string TcpConnection::read_data()
{
	if (error_flag_)
		return "";
	std::unique_lock<std::mutex> lk{ mut_ };
	while (read_msg_deque_.empty())
		data_cond_.wait(lk);
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void TcpConnection::do_close()
{
	try {
		error_flag_ = true;
		chat_message empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		std::cout << "before notify";
		data_cond_.notify_one();
		asio::error_code ec;
		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		if (!ec)
		{
			throw asio::system_error(ec);
		}
		socket_.close();
	}
	catch (std::exception&e)
	{
		e.what();
	}
	//delete_from_parent();
}

void TcpConnection::handle_read_header(const asio::error_code& error)
{
	if (!error && read_msg_.decode_header())
	{
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
		std::cout << "here\n";
		asio::async_read(socket_,
			asio::buffer(read_msg_.body(), read_msg_.body_length()),
			std::bind(&TcpConnection::handle_read_body, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

void TcpConnection::handle_read_body(const asio::error_code& error)
{
	if (!error)
	{
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
		std::lock_guard<std::mutex> lk{ mut_ };
		read_msg_deque_.push_back(read_msg_);
		data_cond_.notify_one();
		asio::async_read(socket_,
			asio::buffer(read_msg_.data(), chat_message::header_length),
			std::bind(&TcpConnection::handle_read_header, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

TcpConnection::TcpConnection(asio::io_service& io_service, chat_server* parent) :
	socket_(io_service), parent(parent)
{
	std::cout << "new tcp" << std::endl;
}
//void TcpConnection::check_timer()
//{
//	if (steady_timer_.expires_at() <= std::chrono::steady_clock::now())
//	{
//		// The deadline has passed. The socket is closed so that any outstanding
//		// asynchronous operations are cancelled.
//		do_close();
//		steady_timer_.expires_at(std::chrono::steady_clock::time_point::max());
//		return;
//	}
//
//	// Put the actor back to sleep.
//	steady_timer_.async_wait(std::bind(&TcpConnection::check_timer, this));
//}

/*
void TcpConnection::delete_from_parent()
{
	if (parent)
		shared_from_this()->parent->remove_connection(shared_from_this());
	parent = nullptr;
}
*/
