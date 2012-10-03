


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

#include "utils/cvector.hpp"
#include "utils/cproducerconsumer.hpp"
#include "utils/platform/sleep.hpp"
#include "test/cproducer.hpp"
#include "test/cconsumer.hpp"

int main(int argc, char *argv[])
{
	int maxquesize, producerCount, consumerCount;
		
	if (argc != 4)
	{
		printf("usage: pctest <maximal queue size> <#producents> <#consuments> \n");
		return -1;
	}
	
	maxquesize = strtol(argv[1], (char **)NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf("cannot parse first parametr - <maximal queue size>\n");
		return -2;
	}
	
	producerCount = strtol(argv[2], (char **)NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf("cannot parse second parametr - <number of producents>\n");
		return -3;
	}
	
	consumerCount = strtol(argv[2], (char **)NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf("cannot parse second parametr - <number of consuments>\n");
		return -4;
	}
	
	if (producerCount <= 0)
	{
		printf("Number of producers must be greater than zero\n");
		return -5;
	}
	
	if (consumerCount <= 0)
	{
		printf("Number of consumers must be greater than zero\n");
		return -6;
	}
	
	// ----------------------------
	
	San2::Utils::CProducerConsumer<int> pc(maxquesize);
	
	San2::Utils::CVector<San2::Test::CProducer<int>*> producers;
	San2::Utils::CVector<San2::Test::CConsumer<int>*> consumers;
	
	for (int i = 0; i < producerCount; i++)
	{
		producers += new San2::Test::CProducer<int>(pc);
	}
	
	for (int i = 0; i < consumerCount; i++)
	{
		consumers += new San2::Test::CConsumer<int>(pc);
	}
	
	printf("start\n");
	
	//consumers[0]->start();
	
	for (unsigned int i = 0; i < consumers.size(); i++) consumers[i]->start();
	for (unsigned int i = 0; i < producers.size(); i++) producers[i]->start();
	
	
	//std::for_each(consumers.begin(), consumers.end(), [] (San2::Test::CConsumer<int>* c) {c->start();});
	//std::for_each(producers.begin(), producers.end(), [] (San2::Test::CProducer<int>* p) {p->start();});
	
	
	for (unsigned k = 0; k < 15; k++)
	{
		San2::Utils::SanSleep(1);
		std::cout << pc.size() << std::endl;
	}
	
	for (unsigned int i = 0; i < consumers.size(); i++) consumers[i]->terminateAndJoin();
	for (unsigned int i = 0; i < producers.size(); i++) producers[i]->terminateAndJoin();
	
	return 0;
}

