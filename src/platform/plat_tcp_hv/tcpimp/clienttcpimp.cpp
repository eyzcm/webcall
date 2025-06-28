#include "service/util_service.h"
#include "clienttcpimp.h"
#include "EventLoopThread.h"
using namespace hv;
bool ClientTcpImp::m_forceClose = false;

ClientTcpImp::ClientTcpImp(QObject * parent)
{
	
	reconn_setting = nullptr;
	connect_timeout = 17000;
	hmutex_init(&this->mutex_);
	
}


void ClientTcpImp::reconnect_timer_cb(htimer_t* timer) {
	if (m_forceClose) return;
	//GTESTLOG("reconnect_timer_cb");
    ClientTcpImp* cli = (ClientTcpImp*)hevent_userdata(timer);
    if (cli == NULL) return;
    cli->reconn_timer = NULL;
	cli->reconn_setting->cur_retry_cnt += 1;
    tcp_client_reconnect(cli);
}

void ClientTcpImp::on_close(hio_t* io) {
	//GTESTLOG(QString("onclose: connfd=%1 error=%2\n").arg(hio_fd(io)).arg(hio_error(io)) );
    
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hio_error(io),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPSTR)&lpMsgBuf,
		0,
		NULL
	);
	std::string str = (LPSTR)lpMsgBuf;
	QString errorMsg = QString(str.c_str());

	

    ClientTcpImp* cli = (ClientTcpImp*)hevent_userdata(io);
    cli->connected = 0;
	cli->OnDisconncted();
    // reconnect
    if (cli->reconn_setting && reconn_setting_can_retry(cli->reconn_setting)) {
        uint32_t delay = reconn_setting_calc_delay(cli->reconn_setting);

		
        cli->reconn_timer = htimer_add(cli->loop, reconnect_timer_cb, delay, 1);
        hevent_set_userdata(cli->reconn_timer, cli);
    }

	
}

void ClientTcpImp::on_message(hio_t* io, void* buf, int len) {
    

    ClientTcpImp* cli = (ClientTcpImp*)hevent_userdata(io);
	cli->OnMessage(buf, len);

}

void ClientTcpImp::OnMessage(void * buf, int len)
{
	Q_EMIT S_message(buf, len);
}

void ClientTcpImp::on_connect(hio_t* io) {
	
    ClientTcpImp* cli = (ClientTcpImp*)hevent_userdata(io);
    cli->connected = 1;
	

#if TEST_UNPACK
    static unpack_setting_t s_unpack_setting;
    s_unpack_setting.mode = UNPACK_BY_DELIMITER;
    s_unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
    s_unpack_setting.delimiter_bytes = 1;
    s_unpack_setting.delimiter[0] = '\0';
    hio_set_unpack(io, &s_unpack_setting);
#endif

    //hio_write(io, "hello\r\n", 7);

    hio_setcb_read(io, on_message);
	cli->OnConnected();
    hio_read(io);
	
}

// hloop_new -> malloc(tcp_client_t)
void ClientTcpImp::tcp_client_new(hloop_t* loop,const char * srchost,int srcport) {
    if (loop == NULL) {
        loop = hloop_new(HLOOP_FLAG_AUTO_FREE);
        if (loop == NULL) return ;
    }

    this->loop = loop;
	strcpy_s(this->host, srchost);
	this->port = srcport;

	reconn_setting_t reconn;
	reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 0xffffffff;
	reconn.delay_policy = 2;
	//reconn.max_retry_cnt = 10;
	tcp_client_set_reconnect( &reconn);


}

// hloop_free -> free(tcp_client_t)
void ClientTcpImp::tcp_client_free() {

    hmutex_destroy(&this->mutex_);
    if (this->reconn_timer) {
        htimer_del(this->reconn_timer);
        this->reconn_timer = NULL;
    }
    if (this->ssl_ctx && this->alloced_ssl_ctx) {
        hssl_ctx_free(this->ssl_ctx);
        this->ssl_ctx = NULL;
    }
    HV_FREE(this->reconn_setting);
    //HV_FREE(this);
}

void ClientTcpImp::tcp_client_run () {

	
    if ( !this->loop) return;
    hloop_run(this->loop);
}

void ClientTcpImp::tcp_client_stop() {
    if (!this->loop) return;
    hloop_stop(this->loop);
}

int ClientTcpImp::tcp_client_set_ssl_ctx( hssl_ctx_t ssl_ctx) {
      this->ssl_ctx = ssl_ctx;
    return 0;
}

// hssl_ctx_new(opt) -> tcp_client_set_ssl_ctx
int ClientTcpImp::tcp_client_new_ssl_ctx(hssl_ctx_opt_t* opt) {
    opt->endpoint = HSSL_CLIENT;
    hssl_ctx_t ssl_ctx = hssl_ctx_new(opt);
    if (ssl_ctx == NULL) return ERR_NEW_SSL_CTX;
    this->alloced_ssl_ctx = true;
    return tcp_client_set_ssl_ctx( ssl_ctx);
}

int ClientTcpImp::tcp_client_set_reconnect(reconn_setting_t* reconn) {
    if (reconn == NULL) {
        HV_FREE(this->reconn_setting);
        return 0;
    }
    if (this->reconn_setting == NULL) {
        HV_ALLOC_SIZEOF(this->reconn_setting);
    }
    *this->reconn_setting = *reconn;
    return 0;
}

int ClientTcpImp::tcp_client_reconnect(ClientTcpImp* cli) {
    cli->tcp_client_connect( cli->host, cli->port, cli->ssl);
    return 0;
}

int ClientTcpImp::tcp_client_connect( const char* srchost, int srcport, int ssl) {

    hv_strncpy(host, srchost, sizeof( host));
    port = srcport;
    this->ssl = ssl;
    hio_t* io = hio_create_socket(loop, host, port, HIO_TYPE_TCP, HIO_CLIENT_SIDE);
	if (io == NULL)
	{
		
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US), // Default language
			(LPSTR)&lpMsgBuf,
			0,
			NULL
		);

		QString errorMsg;
		
		if(lpMsgBuf ) errorMsg  = (LPSTR)lpMsgBuf;
        OnNetWorkError(GetLastError(), errorMsg);
		if (this->reconn_setting && reconn_setting_can_retry(this->reconn_setting)) {
			uint32_t delay = reconn_setting_calc_delay(this->reconn_setting);
			this->reconn_timer = htimer_add(this->loop, reconnect_timer_cb, delay, 1);
			hevent_set_userdata(this->reconn_timer, this);
		}
		return -1;

	}
    if (ssl) {
        if ( ssl_ctx) {
            hio_set_ssl_ctx(io, ssl_ctx);
        }
        hio_enable_ssl(io);
    }
    if ( connect_timeout > 0) {
        hio_set_connect_timeout(io, connect_timeout);
    }
    this->io = io;
    hevent_set_userdata(io, this);
    hio_setcb_connect(io, on_connect);
    hio_setcb_close(io, on_close);
    return hio_connect(io);
}

int ClientTcpImp::tcp_client_disconnect() {
    if (! this->io) return -1;
    // cancel reconnect first
    tcp_client_set_reconnect( NULL);
    return hio_close(this->io);
}

bool ClientTcpImp::tcp_client_is_connected() {
    return   this->connected;
}

int ClientTcpImp::tcp_client_send( const void* buf, int len) {
    if ( ! this->io || !buf || len == 0) return -1;
    if (!this->connected) return -2;
    // thread-safe
    hmutex_lock(&this->mutex_);
    int nwrite = hio_write(this->io, buf, len);
    hmutex_unlock(&this->mutex_);
    return nwrite;
}


void ClientTcpImp::OnConnected()
{
	Q_EMIT S_connected();
	
}
void ClientTcpImp::OnDisconncted()
{
	Q_EMIT S_disconnected();
}

void ClientTcpImp::OnNetWorkError(int code, QString errormsg)
{
	Q_EMIT S_NetWorkError(code,  errormsg);
}
