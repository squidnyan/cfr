#/bin/bash
clear; scons && clear && ./cfr test/files/Test.class test/files/RabbitQueueClient.class test/files/RabbitQueueConsumer.class /dev/urandom test/files/LongTest.class test/files/DoubleTest.class test/files/Interfaces.class
