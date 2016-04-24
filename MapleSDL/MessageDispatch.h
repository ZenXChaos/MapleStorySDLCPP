

class MD_Time {
public:
	Uint32 birth;

	Uint32 GetAge(Uint32 tick);
};

class MessageDispatch {

public:

	std::map<std::string, MD_Time> messages;

	void ReceiveMessage(std::string msg, MD_Time md) {

	}
	
	MD_Time* getMessage(std::string msg) {
		std::map<std::string, MD_Time>::iterator it;

		it = messages.find(msg);

		if (it == messages.end()) {
			return nullptr;
		}else{
			return &this->messages[msg];
		}
	}


	MessageDispatch() {
		
	}
};