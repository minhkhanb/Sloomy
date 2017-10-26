
pthread_t _data_server_thread;

void *ServerDataRun(void* param)
{
    //CGame::_this->_data_sever_manager.UpdateServer();
    GAME()->_serverThread.ServerRun();
}

extern "C" void DataSeverStart()
{
	if (pthread_create(&_data_server_thread, NULL, &ServerDataRun, NULL))
	{
		exit(1);
	}
}