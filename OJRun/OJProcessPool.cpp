#include "./OJProcessPool.h"

#include <ctime>
#include <queue>
#include <string>
#include <vector>

#if __linux__

#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/resource.h>

#endif

#include "./OJFileCheck.h"
#include "../OJSql/OJSql.h"
#include "../OJUtils/OJDebug.h"
#include "../OJUtils/OJVariant.h"

OJ_NAMESPACE_BEGIN


#if __linux__

struct task_t
{
    int usrID;
    int questionID;
    OJJudgeLanguage lang;
    int memLimit, timeLimit;
    std::string codePath;
    std::vector<std::string> inFiles, outFiles;
};

struct process_t 
{
    pid_t pid;
};

#endif

// struct task_t 
// {
//     int usrID;
//     int timeLimit;
//     int questionID;
//     int memoryLimit;
//     OJJudgeLanguage lang;
//     std::string codePath;
//     std::vector<std::string> inFiles, outFiles;
//     std::string sampleInputPath, sampleOutputPath;

//     task_t(int uid, int qid, int tLimit, int mLimit, const std::string& c, OJJudgeLanguage lang)
//         : usrID(uid), timeLimit(tLimit), questionID(qid), memoryLimit(mLimit), codePath(c), lang(lang) {
// #if __linux__
//         // 读取输入文件
//         DIR *inDir = opendir(sampleInputPath.c_str());
//         if (inDir != NULL) {
//             dirent* entry;
//             while ((entry = readdir(inDir)) != NULL) {
//                 if (entry->d_type == DT_REG) {
//                     inFiles.push_back(entry->d_name);
//                 }
//             }
//         }
//         closedir(inDir);
        
//         // 读取输出文件
//         DIR *outDir = opendir(sampleOutputPath.c_str());
//         if (outDir != NULL) {
//             dirent* entry;
//             while ((entry = readdir(outDir)) != NULL) {
//                 if ((entry->d_type == DT_REG)) {
//                     outFiles.push_back(entry->d_name);
//                 }
//             }
//         }
//         closedir(outDir);
// #elif _WIN32_
// #endif
//     }

    
// };

// #if __linux__

// class OJProcessPoolPrivate
// {
//     friend class OJProcessPool;
// public:
//     OJProcessPoolPrivate(int num, judge_server_result register_func);

//     void join(const std::vector<std::string>& code, int uid, int qid, OJJudgeLanguage lang=OJJudgeLanguage::CPLUSPLUS11);
// private:
//     int workNum;
//     int totalNum;
//     OJLogger logger;
//     OJSqlConnector connector;
//     std::vector<pid_t> process;
//     sem_t pJoin, pCheck, mutex, logMutex;
//     std::queue<task_t> workTasks, waitTasks;
//     OJFileCheck cplusplusCheck, cCheck, javaCheck, python2Check, python3Check;
// private:
//     void run();
//     judge_server_result resulFunc;
//     std::string getGccOptions(OJJudgeLanguage lang);
//     std::string getGcpluscplusOptions(OJJudgeLanguage lang);
// };

// OJProcessPoolPrivate::OJProcessPoolPrivate(int num, judge_server_result register_func)
//     : workNum(0), totalNum(num), resulFunc(register_func)
// {
//     logger = OJLogger("/home/linux2023/onlinejudge/log/startup.log");
//     sem_init(&pCheck, 1, 0);
//     sem_init(&pJoin, 1, 1); sem_init(&mutex, 1, 1); sem_init(&logMutex, 1, 1);

//     connector = OJSqlConnector("OJMYSQL");
//     connector.setPassword("123"); connector.setUserName("oj_manager");
//     connector.setDatabase("onlinejudge"); connector.setHost("localhost");

//     cplusplusCheck = OJFileCheck("C++"); cCheck = OJFileCheck("C");

//     for (int i = 0;i < totalNum;i ++ ) {
//         pid_t pid = fork();

//         if (pid == -1) {
//             sem_wait(&logMutex);
//             logger.print("OJProcessPool::OJProcessPool fork failed.", OJLogger::WARNING);
//             sem_post(&logMutex);
//             exit(EXIT_FAILURE);
//         }

//         if (pid == 0) {
//             while (true) {
//                 sem_wait(&pCheck);
//                 sem_wait(&pJoin);
//                 run();
//                 sem_post(&pJoin);
//                 sem_post(&pCheck);
//             }
//         } else {    // 主进程
//             process.push_back(pid);
//         }
//     }

// }

// void OJProcessPoolPrivate::join(const std::vector<std::string>& code, int uid, int qid, OJJudgeLanguage lang)
// {
//     // 代码API和import静态检查
//     switch (lang)
//     {
//     case OJJudgeLanguage::CPLUSPLUS98:
//     case OJJudgeLanguage::CPLUSPLUS11:
//     case OJJudgeLanguage::CPLUSPLUS14:
//     case OJJudgeLanguage::CPLUSPLUS17:
//         if (cplusplusCheck.check(code) == OJFileCheck::IMPORT_ERROR) {
//             resulFunc(cplusplusCheck.lastError().c_str(), OJJudgeState::NOT_STD_IMPORT);
//             return ;
//         } else if (cplusplusCheck.check(code) == OJFileCheck::API_ERROR) {
//             resulFunc(cplusplusCheck.lastError().c_str(), OJJudgeState::NOT_STD_API);
//             return ;
//         }
//         break;
//     case OJJudgeLanguage::C99:
//     case OJJudgeLanguage::C11:
//         if (cCheck.check(code) == OJFileCheck::IMPORT_ERROR) {
//             resulFunc(cCheck.lastError().c_str(), OJJudgeState::NOT_STD_IMPORT);
//             return ;
//         } else if (cCheck.check(code) == OJFileCheck::API_ERROR) {
//             resulFunc(cCheck.lastError().c_str(), OJJudgeState::NOT_STD_API);
//             return ;
//         }
//         break;
//     }

//     // 获取时间和空间限制
//     char *sql=new char[100];
//     snprintf(sql, 100, "SELECT * FROM problem WHERE ProblemID=%d;", qid);
//     connector.exec(sql);
//     OJSqlResult result=connector.result();
    
//     if (!result.cols()) {
//         resulFunc("A server internal error occur, please submission now.", OJJudgeState::SERVER_INTERNAL_ERROR);
//         return ;
//     }

//     int timeLimit, memoryLimit;
//     result.first();
//     timeLimit = result.value("ProblemTimeLimit").toInt();
//     memoryLimit = result.value("ProblemMemoryLimit").toInt();

//     std::ofstream infile;
//     std::string compilerPath;
//     char *filename=new char[512];
//     snprintf(filename, 512, "%d-%d-%d-%ld", qid, uid, lang, time(NULL));
//     std::string fileContent(filename); 
//     compilerPath = "/home/linux2023/complier/"+fileContent;
//     infile.open(compilerPath, std::ios::in);

//     for (auto line : code) {
//         infile << line;
//     }

//     task_t task(uid, timeLimit, qid, memoryLimit, compilerPath, lang);

//     sem_wait(&mutex);
//     workTasks.push(task);
//     sem_post(&mutex);

//     delete []sql;
//     delete []filename;

//     sem_post(&pJoin); sem_wait(&pCheck);
// }

// std::string OJProcessPoolPrivate::getGccOptions(OJJudgeLanguage lang)
// {
//     std::string options=" -pedantic -Wall -Werror-implicit-function-declaration ";
//     switch (lang)
//     {
//     case OJJudgeLanguage::C99:
//         options += " -std=c99 "; break;
//     case OJJudgeLanguage::C11:
//         options += " -std=c11 "; break;
//     }
//     return options;
// }

// std::string OJProcessPoolPrivate::getGcpluscplusOptions(OJJudgeLanguage lang)
// {
//     std::string options=" -pedantic -Wall -Werror-implicit-function-declaration ";
//     switch (lang)
//     {
//     case OJJudgeLanguage::CPLUSPLUS98:
//         options += " -std=c++98 "; break;
//     case OJJudgeLanguage::CPLUSPLUS11:
//         options += " -std=c++11 "; break;
//     case OJJudgeLanguage::CPLUSPLUS14:
//         options += " -std=c++14 "; break;
//     case OJJudgeLanguage::CPLUSPLUS17:
//         options += " -std=c++17 "; break;
//     }
//     return options;
// }

// void OJProcessPoolPrivate::run()
// {
//     sem_wait(&mutex);
//     task_t task = workTasks.front();
//     workTasks.pop();
//     sem_post(&mutex);

//     OJJudgeLanguage language=task.lang;

//     // 编译代码
//     auto c_complier_func = [](task_t& task, judge_server_result resultRegisterFunc, const std::string& complierOptions) {
//         int exit_code;
//         std::string output;
//         std::ostringstream std_output;
//         std::string c_complier_path = "gcc " + complierOptions + " " + task.codePath + ".c " + task.codePath + "2>&1";
//         exit_code = std::system(c_complier_path.c_str());
//         output = std_output.str();
//         if (!exit_code) {
//             resultRegisterFunc(output.c_str(), OJJudgeState::COMPLIER_ERROR);
//             return false;
//         }
//         return true;
//     };

//     auto java_complier_func = [](task_t& task) {

//     };

//     auto python_complier_func = [](task_t& task) {

//     };

//     auto cplusplus_complier_func = [](task_t& task, judge_server_result resultRegisterFunc, const std::string& complierOptions) -> bool {
//         int exit_code;
//         std::string output;
//         std::ostringstream std_output_stream;
//         std::string cplusplus_complier_commend = "g++ " + task.codePath + ".cpp -o " + task.codePath + "2>&1";
//         exit_code = std::system(cplusplus_complier_commend.c_str());
//         if (!exit_code) {
//             resultRegisterFunc(std_output_stream.str().c_str(), OJJudgeState::COMPLIER_ERROR);
//             return false;
//         }
//         return true;
//     };

//     // 编译代码成可执行文件
//     switch (task.lang)
//     {
//     case OJJudgeLanguage::C99:
//     case OJJudgeLanguage::C11:
//         if (!c_complier_func(task, resulFunc, getGccOptions(task.lang))) {
//             return ;
//         } 
//         break;
//     case OJJudgeLanguage::CPLUSPLUS98:
//     case OJJudgeLanguage::CPLUSPLUS11:
//     case OJJudgeLanguage::CPLUSPLUS14:
//     case OJJudgeLanguage::CPLUSPLUS17:
//         if (!cplusplus_complier_func(task, resulFunc, getGcpluscplusOptions(task.lang))) {
//             return ;
//         }
//         break;
//     }

//     // 设置内存限制
//     rlim_t memoryLimitBytes = static_cast<rlim_t>(memoryLimitBytes) * 1024 * 1024 * task.memoryLimit;
//     struct rlimit memLimit;
//     memLimit.rlim_cur = memoryLimitBytes; memLimit.rlim_max = memoryLimitBytes;
//     setrlimit(RLIMIT_AS, &memLimit);

//     // 设置CPU运行时间限制
//     struct rlimit timeLimit;
//     timeLimit.rlim_cur = static_cast<rlim_t>(task.timeLimit);
//     timeLimit.rlim_max = static_cast<rlim_t>(task.timeLimit);
//     setrlimit(RLIMIT_CPU, &timeLimit);

//     // 开始执行任务
    
// }

// #endif

// OJProcessPool::OJProcessPool(int num, judge_server_result register_func)
// {
//     vptr = new OJProcessPoolPrivate(num, register_func);
// }

// void OJProcessPool::join(const std::vector<std::string>& code, int uid, int pid, OJJudgeLanguage lang) 
// {
//     vptr->join(code, uid, pid, lang);
// }

OJ_NAMESPACE_END