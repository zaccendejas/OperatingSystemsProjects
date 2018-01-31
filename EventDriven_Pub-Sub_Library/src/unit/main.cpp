// SYSTEM INCLUDES

#include <gtest/gtest.h>

// C++ PROJECT INCLUDES

#include "ps_client/client.h"

namespace {
	class ClientTest : public ::testing::Test {
		protected:
			virtual void SetUp(){
				Client01.OutgoingMessages.push("Test Input01\n");

			}

			virtual void TearDown(){

			}

			const char *Host = "LocalHost";
			const char *Port = "9888";
			const char *CID = "zac";
			Client::Client Client01(Host, Port, CID);
	
	};

	// Tests that publish updates the outgoing queue
	TEST_F(ClientTest, MethodAddsPush){
		const char *Topic = "PolarBears";
		const char *Message = "Polar Bears are ice cold\n";
		size_t Length = strlen(Message);
		Client01.publish(Topic, Message, Length);
		EXPECT_EQ(2, Client01.OutgoingMessages.Correspondence.size());
	}

	// Tests that subscribe updates the outgoing queue and the callbacktable
	TEST_F(ClientTest, MethodAddsSub){
		const char *Topic = "PolarBears";
		Callback Testcallback;
		Client01.subscribe(Topic, &Testcallback);
		EXPECT_EQ(2, Client01.OutgoingMessages.Correspondence.size());
		EXPECT_EQ(1, Client01.CallbackTable.size());
	}

	// Tests that unsubscribe updates the outgoing queue
	TEST_F(ClientTest, MethodAddsUnsub){
		const char *Topic = "PolarBears";
		Client01.unsubscribe(Topic);
		EXPECT_EQ(2, Client01.OutgoingMessages.Correspondence.size());
	}

	// Tests that disconnect updates the outgoing queue
	TEST_F(ClientTest, MethodAddsDis){
		Client01.disconnect();
		EXPECT_EQ(2, Client01.OutgoingMessages.Correspondence.size());
	}

	// Tests that run processes all queues
	TEST_F(ClientTest, MethodRuns){
		Client01.run();
		EXPECT_EQ(0, Client01.OutgoingMessages.Correspondence.size());
		EXPECT_EQ(0, Client01.IncomingMessages.Correspondence.size());
		EXPECT_EQ(0, Client01.CallbackTable.size());
	}

}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
