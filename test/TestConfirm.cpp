#include "src\Confirmation.h"

#include "src\ReportResult.h"
#include "src\AssertException.h"

#include "AutoRun.h"

namespace TestAsserts{
	class Base: public Auto::TestCase, gppUnit::ReportResult{ 
		gppUnit::TestResult testResult;
		virtual void Report(const gppUnit::TestResult& result){
			testResult=result;
		}
	protected:
		void setReport(gppUnit::ResultSetter& setter){
			setter.setReport(this);
		}
		void thenDescriptionIsEmpty(){
			confirm.equals(0,testResult.description.size(),"Description is empty");
		}
		void thenDescriptionIs(const std::string& desc){
			expect.equals(1,testResult.description.size(),"Description has one line");
			confirm.equals(desc,testResult.description.front(),desc.c_str());
		}
		void thenResultIsFail(){
			confirm.isFalse(testResult.result,"thenResultIsFail");
			confirm.equals("fail",testResult.message,"message is fail");
			thenDescriptionIsEmpty();
		}
		void thenResultIsPass(){
			confirm.isTrue(testResult.result,"thenResultIsPass");
			confirm.equals("pass",testResult.message,"message is pass");
			thenDescriptionIsEmpty();
		}
		void thenResultIsFalse(const char* message){
			confirm.isFalse(testResult.result,"thenResultIsFalse");
			confirm.equals(message,testResult.message,message);
			thenDescriptionIsEmpty();
		}
		void thenResultIsTrue(const char* message){
			confirm.isTrue(testResult.result,"thenResultIsTrue");
			confirm.equals(message,testResult.message,message);
			//thenDescriptionIsEmpty();
		}
	};
	class ConfirmPassAndFail: public Base{
		gppUnit::Confirm conf;
		void givenConfirm(){
			setReport(conf);
		}
		void whenFailCalled(){
			conf.fail("fail");
		}
		void whenPassCalled(){
			conf.pass("pass");
		}
		void test(){
			givenConfirm();
			whenFailCalled();
			thenResultIsFail();
			whenPassCalled();
			thenResultIsPass();
		}
	}GPPUNIT_INSTANCE;
	class ConfirmIsTrue: public Base{
		gppUnit::Confirm conf;
		void givenConfirm(){
			setReport(conf);
		}
		void whenIsTrueCalled(bool value){
			conf.isTrue(value);
		}
		void test(){
			givenConfirm();
			whenIsTrueCalled(false);
			thenResultIsFalse("Should be True");
			whenIsTrueCalled(true);
			thenResultIsTrue("Should be True");
		}
	}GPPUNIT_INSTANCE;

	class CatchExceptionsFromExpect: public Base{
		gppUnit::Expect exp;
		bool caughtException;
		void givenExpect(){
			setReport(exp);
			caughtException=false;
		}
		void whenFailCalled(){
			try{
				exp.fail("fail");
			} catch (gppUnit::AssertException e){
				caughtException=true;
			}
		}
		void thenExceptionCaught(){
			confirm.isTrue(caughtException,"thenExceptionCaught");
		}
		void whenPassCalled(){
			exp.pass("pass");
		}
		void test(){
			givenExpect();
			whenFailCalled();
			thenResultIsFail();
			thenExceptionCaught();
			whenPassCalled();
			thenResultIsPass();
		}
	}GPPUNIT_INSTANCE;

	class ConfirmThat: public Base{
		gppUnit::Confirm conf;
		void givenConfirm(){
			setReport(conf);
		}
		void test(){
			givenConfirm();
			conf.that(0,gppUnit::equal_to(0));
			thenResultIsTrue("Should match");
			thenDescriptionIs("'0'");
		}
	}GPPUNIT_INSTANCE;
}
