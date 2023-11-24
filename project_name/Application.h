/*********************************************************************
 * @file  Apllication.h
 * @author <mettre l'adresse mail ou nom prenom>
 * @brief Fichier header de l'application
 *********************************************************************/
#ifndef APPLICATION_H_
#define APPLICATION_H_

/**
  * @class Application
  * @brief Classe Application 
*/    
class Application
{
  public :
    /**
     * @fn Application();
     * @brief Constructeur par defaut
    */    
    Application();
    /**
     * @fn Application();
     * @brief Destructeur
    */    
    ~Application();    
    /**
     * @fn void init_application(void)
     * @brief Fonction d'initialisation de l'application
    */
    void init_application(void);
    /**
     * @fn void run_appplication(void)
     * @brief Fonction de lancement de l'application
    */
    void run_appplication(void);
};
#endif