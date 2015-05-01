set(project_name MMO7_binder)

project(${project_name})

cmake_minimum_required (VERSION 2.6)

#set compiler flags
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 -Wall")
#set Variables
set(EXECUTABLE_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_PATH}/${project_name})


#grab files
file(GLOB src_deamon "src/MMO7binder.cpp"
                     "src/mmo7_binder.cpp"
					      )
                   
file(GLOB src_get_id "src/mmo7_get_btn_id.cpp"
					    )
                    
              
#add executable
add_executable(mmo7_binder ${src_deamon})
add_executable(mmo7_get_btn_id ${src_get_id})

#link executable
target_link_libraries(mmo7_binder tinyxml2)


#install
install(TARGETS mmo7_binder
        RUNTIME DESTINATION bin
        )                      
           
install(TARGETS mmo7_get_btn_id
        RUNTIME DESTINATION bin
        )
        
install(FILES config/mmo7_binder_cfg.xml DESTINATION /etc/mmo7_binder/)          