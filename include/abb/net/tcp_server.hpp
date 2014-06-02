#ifndef __ABB_NET_TCP_SERVER_HPP__
#define __ABB_NET_TCP_SERVER_HPP__

#include "./ip_addr.hpp"
#include "./listener.hpp"
namespace abb{
	namespace net{
		class Context;
		class TcpServer:public IAcceptorListener,IConnectionListener{
		public:
			class Listener{
			public:
				~Listener(){}
				virtual void L_TcpServer_OnConnection();
				virtual void L_TcpServer_OnMesssage();
				virtual void L_TcpServer_OnClose();
			};
			TcpServer(const IPAddr& addr);
			virtual ~TcpServer();
			void SetNumThread(int num);
			void SetListener(Listener* lis){lis_ = lis;}
			bool Bind(int* save_error);
			void Start(bool run_cur_thread);
			virtual void L_Connector_OnOpen(Connection* cotr,Connection* conn)=0;
			virtual void L_Connector_OnClose(Connection* cotr)=0;
			virtual void L_Acceptor_OnConnection(Acceptor* ptr,Connection* conn) = 0;
		private:
			Listener* lis_;
			Context* ctx_;
			Acceptor* acceptor_;
			IPAddr addr_;
		};
	}
}

#endif