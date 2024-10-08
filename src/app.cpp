#include <iostream>
#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"

#include "tinyxml2.h"
#include <vector>
#include <string>
#include "special_agent.hpp"

class InitManager{
public:
    InitManager(){ }
    InitManager(std::string path) { 
        tinyxml2::XMLDocument doc;  // xml 파일 읽어오기
        doc.LoadFile(path.c_str()); // 다음 path에서
        tinyxml2::XMLElement* agListElem = doc.FirstChildElement("scenario")->FirstChildElement("AgentList");   // scenario 안에 있는 AgentList Element를 찾아서

        double x, y, heading, speed, drange;
        for(tinyxml2::XMLElement* agElem = agListElem->FirstChildElement(); // AgentList Element의 첫번째 자식 Element부터
            agElem != NULL; agElem = agElem->NextSiblingElement()){         // 다음 Element로 비어있을 때까지 반복
                agElem->QueryDoubleAttribute("x", &x);                      // 각 Element의 x, y, heading, speed, drange를 불러옴
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);
                CAgent* ag = new CSpecialAgent(x,y,heading,speed,drange);   // 각 Element의 x, y, heading, speed, drange를 매개변수로 넘겨주고 CSpecialAgent 객체 생성
                m_agent_list.push_back(ag);                                 // 생성된 객체를 m_agent_list에 추가
                // std::cout << x << ", " << y << std::endl;
            }

    }
private:
    std::vector<CAgent*> m_agent_list;                                      // CAgent 객체를 담을 vector 생성
public:
    std::vector<CAgent*>& get_agent_list() {return m_agent_list;}           // m_agent_list를 반환하는 함수 (함수를 통해서만 접근 가능)
};

int main(int argc, char** argv)
{// Clinet Code  
    InitManager init_manager("test.xml");   // test.xml 경로를 InitManager에 넣어서 초기화

    /* Simulation Engine */
    CManeuverManager mmanager;              // CManeuverManager 객체 생성

    for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();        // InitManager에서 get_agent_list를 받아와서 iterator로 반복
        iter != init_manager.get_agent_list().end(); ++iter){
            mmanager.register_publisher(*iter);                                             // mmanager에 register_publisher로 이벤트를 발생시킨 *iter를 넣어줌
        }


    // std::vector<CAgent*> m_agent_list;


    /* Simulation Model Registration */
    // CAgent* p_ag = ???.create_agent(10,  0, 0, 1, 10)
    // m_agent_list.push_back(p_ag);
    // CAgent* p_agent1 = new CAgent(10,  0, 0, 1, 10);
    // p_agent1 = new StochasticDecorator(p_agent1);
    // p_agent1 = new SimpleDecorator(p_agent1);
    
    // mmanager.register_publisher(p_agent1);

    // CAgent* p_agent2 = new CAgent(10,  30, 180, 1, 10);
    // mmanager.register_publisher(p_agent2);
    
    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;   // 1 만큼 이동

    for(sim_time = 0; sim_time < 30; sim_time += time_step)
    {   
        //p_agent1->maneuver(time_step);
        //p_agent2->maneuver(time_step);

        //p_agent1->detect(p_agent2);
        //p_agent2->detect(p_agent1);


        // for(std::vector<CAgent*>::iterator iter1 = init_manager.get_agent_list().begin();
        //     iter1 != init_manager.get_agent_list().end(); ++iter1){
        //         for(std::vector<CAgent*>::iterator iter2 = init_manager.get_agent_list().begin();
        //             iter2 != init_manager.get_agent_list().end(); ++iter2){
        //             if (iter1 != iter2) {
        //                 (*iter1)->detect(*iter2);
        //             }
        //         }
        //     }

        mmanager.svc(time_step);    // mmanager의 svc 함수를 time_step만큼 실행 (detect)

        // std::cout << "----" << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent1 << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent2 << std::endl;

        std::cout << "---" << std::endl;
        for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();        // InitManager에서 get_agent_list를 받아와서 iterator로 반복
        iter != init_manager.get_agent_list().end(); ++iter){                                   // 함으로써, p_agent를 직접 입력하지 않아도 됨
            
            std::cout << "Time: " << sim_time << ", " << *(*iter) << std::endl;                 // 시간과 InitManager에서 받아온 *iter 출력
        }
    }
    return 0;
}

/*
 - 클라이언트 코드에 왜 클래스 타입을 몰라도 동작하는 지 설명 (힌트: 다형성)
 
    같은 인터페이스를 통해 다양한 타입의 객체를 다룰 수 있게 해주는 다형성 때문임.
    CAgent 는 기본 클래스이고, CSpecialAgent나 이전에 사용했던 StochasticDecorator 또는 SimpleDecorator 모두 CAgent를 상속받은 파생 클래스로
    CAgent의 포인터로 파생 클래스들의 객체를 가리킬 수 있기 때문에, 클라이언트 코드는 CAgent의 포인터만 알고 있어도 파생 클래스들 객체를 다룰 수 있음.

*/