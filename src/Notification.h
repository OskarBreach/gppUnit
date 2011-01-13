#ifndef NOTIFICATION_H_FE7AAFFA_316E_4165_9D82_C593A049C539
#define NOTIFICATION_H_FE7AAFFA_316E_4165_9D82_C593A049C539

namespace gppUnit{
	class MethodDescription;

	class Notification{
	protected:
		virtual ~Notification(){}
	public:
		virtual void StartMethod(const MethodDescription&){}
		virtual void EndMethod(){}
	};
}

#endif // NOTIFICATION_H_FE7AAFFA_316E_4165_9D82_C593A049C539