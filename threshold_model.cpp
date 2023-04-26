#include "threshold_model.h"
#include "buzz/buzzvm.h"

static CRange<Real> STIMULUS_RANGE(-10.0, 10.0);

/****************************************/
/****************************************/

/**
 * Functor to get data from the robots
 */
struct GetRobotData : public CBuzzLoopFunctions::COperation {

   /** Constructor */
   GetRobotData(int n) : m(n ,0) {}

   /** The action happens here */
   virtual void operator()(const std::string& str_robot_id,
                           buzzvm_t t_vm) {
      /* Get the current task */
      buzzobj_t tTask = BuzzGet(t_vm, "happiness");
      /* Make sure it's the type we expect (an integer) */
      if(!buzzobj_isfloat(tTask)) {
         LOGERR << str_robot_id << ": variable 'happiness' has wrong type " << buzztype_desc[tTask->o.type] << std::endl;
         return;
      }
      /* Get the value */
      float nTask = buzzobj_getfloat(tTask);
      /* Make sure its value is correct */
      if(nTask < 0) { // >= m_vecTaskCounts.size()) {
         LOGERR << str_robot_id << ": variable 'happiness' has wrong value " << nTask << std::endl;
         return;
      }
      /* Set the mapping */
      m_vecRobotsTasks[t_vm->robot] = nTask;

      /* Get the current task */
      buzzobj_t tSwarm = BuzzGet(t_vm, "my_swarm");
      /* Make sure it's the type we expect (an integer) */
      if(!buzzobj_isint(tTask)) {
         LOGERR << str_robot_id << ": variable 'my_swarm' has wrong type " << buzztype_desc[tTask->o.type] << std::endl;
         return;
      }
      /* Get the value */
      int nSwarm = buzzobj_getint(tSwarm);
      /* Make sure its value is correct */
      if(nSwarm < 0) { // >= m_vecTaskCounts.size()) {
         LOGERR << str_robot_id << ": variable 'my_swarm' has wrong value " << nTask << std::endl;
         return;
      }
      /* Set the mapping */
      m_vecRobotsSwarms[t_vm->robot] = nSwarm;

      /* Get the current thresholds */
      BuzzTableOpen(t_vm, "position");
      buzzobj_t tThreshold = BuzzGet(t_vm, "position");
      /* Make sure it's the type we expect (a table) */
      if(!buzzobj_istable(tThreshold)) {
         LOGERR << str_robot_id << ": variable 'position' has wrong type " << buzztype_desc[tThreshold->o.type] << std::endl;
         return;
      }
      /* Get the values */
      m_vecRobotsThresholds[t_vm->robot].resize(2, 0.0);
      for(int i = 0; i < 2; ++i) {
         /* Get the object */
         buzzobj_t tThresholdValue = BuzzTableGet(t_vm, i);
         /* Make sure it's the type we expect (a float) */
         if(!buzzobj_isfloat(tThresholdValue)) {
            LOGERR << str_robot_id << ": element 'position[" << i << "]' has wrong type " << buzztype_desc[tThresholdValue->o.type] << std::endl;
         }
         else {
            /* Get the value */
            float fThresholdValue = buzzobj_getfloat(tThresholdValue);
            /* Set the mapping */
            m_vecRobotsThresholds[t_vm->robot][i] = fThresholdValue;
         }
      }
   }

   std::vector<int> m;
   /* Task-robot mapping */
   std::map<int,float> m_vecRobotsTasks;
   /* Task-robot mapping */
   std::map<int,int> m_vecRobotsSwarms;
   /* Robot-threshold mapping */
   std::map<int,std::vector<float> > m_vecRobotsThresholds;
};

/****************************************/
/****************************************/

void CThresholdModel::Init(TConfigurationNode& t_tree) {
   /* Call parent Init() */
   CBuzzLoopFunctions::Init(t_tree);
   /* Parse XML tree */
   GetNodeAttribute(t_tree, "outfile", m_strOutFile);
   /* Create a new RNG */
   m_pcRNG = CRandom::CreateRNG("argos");
   /* Open the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);
}

/****************************************/
/****************************************/

void CThresholdModel::Reset() {
   /* Reset the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);

}

/****************************************/
/****************************************/

void CThresholdModel::Destroy() {
   m_cOutFile.close();
}

/****************************************/
/****************************************/

void CThresholdModel::PostStep() {
   /* Get robot data */
   GetRobotData cGetRobotData(2);
   BuzzForeachVM(cGetRobotData);
   /* Flush data to the output file */
   if(!cGetRobotData.m_vecRobotsThresholds.empty()) {
      for(int i = 0; i < GetNumRobots(); ++i) {
         m_cOutFile << GetSpace().GetSimulationClock() << "\t"
                    << i << "\t"
                    << cGetRobotData.m_vecRobotsSwarms[i]
                    << "\t"
                    << cGetRobotData.m_vecRobotsTasks[i];
         for(int j = 0; j < 2; ++j) {
            m_cOutFile << "\t" << cGetRobotData.m_vecRobotsThresholds[i][j];
         }
         m_cOutFile << std::endl;
      }
   }
}

/****************************************/
/****************************************/

bool CThresholdModel::IsExperimentFinished() {
   /* Feel free to try out custom ending conditions */
   return false;
}

/****************************************/
/****************************************/

int CThresholdModel::GetNumRobots() const {
   return m_mapBuzzVMs.size();
}

/****************************************/
/****************************************/

void CThresholdModel::BuzzBytecodeUpdated() {
   /* Convey the stimuli to every robot */
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CThresholdModel, "threshold_model");
