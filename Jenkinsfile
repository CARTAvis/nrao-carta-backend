void setBuildStatus(String message, String state) {
  step([
      $class: "GitHubCommitStatusSetter",
      reposSource: [$class: "ManuallyEnteredRepositorySource", url: "https://github.com/CARTAvis/carta-backend"],
      contextSource: [$class: "ManuallyEnteredCommitContextSource", context: "ci/jenkins/build-status"],
      errorHandlers: [[$class: "ChangingBuildStatusErrorHandler", result: "UNSTABLE"]],
      statusResultSource: [ $class: "ConditionalStatusResultSource", results: [[$class: "AnyBuildResult", message: message, state: state]] ]
  ]);
}

pipeline {
    agent none
    stages {
        stage('Build') {
            parallel {
                stage('CentOS7 build') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        sh "export PATH=/usr/local/bin:$PATH"
                        sh "git submodule update --init --recursive"
                        dir ('build') {
                           sh "rm -rf *"
                           sh "cp ../../cmake-command.sh ."
                           sh "./cmake-command.sh"
                           sh "make -j\$(nproc)"
                           echo "Preparing for upcoming ICD tests"
                           sh "rm -rf carta-backend-ICD-test"
                           sh "git clone --depth 1 https://github.com/CARTAvis/carta-backend-ICD-test.git && cp ../../run.sh ."
                           dir ('carta-backend-ICD-test') {
                              sh "git submodule init && git submodule update && npm install"
                              dir ('protobuf') {
                                 sh "./build_proto.sh"
                              }
                              sh "cp ../../../ws-config.json src/test/config.json"
                           }
                           stash includes: "carta_backend", name: "centos7-1_carta_backend_icd"
                        }
                    }
                    post {
                        success {
                            setBuildStatus("CentOS7 build succeeded", "SUCCESS");
                        }
                        failure {
                            setBuildStatus("CentOS7 build failed", "FAILURE");
                        }
                    }
                }
                stage('MacOS build') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        sh "export PATH=/usr/local/bin:$PATH"
                        sh "git submodule update --init --recursive"
                        dir ('build') {
                           sh "rm -rf *"
                           sh "cp ../../cmake-command.sh ."
                           sh "./cmake-command.sh"
                           sh "make -j\$(nproc)"
                           echo "Preparing for upcoming ICD tests"
                           sh "rm -rf carta-backend-ICD-test"
                           sh "git clone --depth 1 https://github.com/CARTAvis/carta-backend-ICD-test.git && cp ../../run.sh ."
                           dir ('carta-backend-ICD-test') {
                              sh "git submodule init && git submodule update && npm install"
                              dir ('protobuf') {
                                 sh "./build_proto.sh"
                              }
                              sh "cp ../../../ws-config.json src/test/config.json"
                           }
                           stash includes: "carta_backend", name: "macos-1_carta_backend_icd"
                        }
                    }
                    post {
                        success {
                            setBuildStatus("MacOS build succeeded", "SUCCESS");
                        }
                        failure {
                            setBuildStatus("MacOS build failed", "FAILURE");
                        }
                    }
                }
                stage('Ubuntu build') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        sh "export PATH=/usr/local/bin:$PATH"
                        sh "git submodule update --init --recursive"
                        dir ('build') {
                           sh "rm -rf *"
                           sh "cp ../../cmake-command.sh ."
                           sh "./cmake-command.sh"
                           sh "make -j\$(nproc)"
                           echo "Preparing for upcoming ICD tests"
                           sh "rm -rf carta-backend-ICD-test"
                           sh "git clone --depth 1 https://github.com/CARTAvis/carta-backend-ICD-test.git && cp ../../run.sh ."
                           dir ('carta-backend-ICD-test') {
                              sh "git submodule init && git submodule update && npm install"
                              dir ('protobuf') {
                                 sh "./build_proto.sh"
                              }
                              sh "cp ../../../ws-config.json src/test/config.json"
                         }
                         stash includes: "carta_backend", name: "ubuntu-1_carta_backend_icd"
                        }
                    }
                    post {
                        success {
                            setBuildStatus("MacOS build succeeded", "SUCCESS");
                        }
                        failure {
                            setBuildStatus("MacOS build failed", "FAILURE");
                        }
                    }
                }
            }
        }
        stage('ICD tests: session') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT.test.ts # test 1 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_KNOWN_SESSION.test.ts # test 2 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_NO_CLIENT_FEATURE.test.ts # test 3 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_SAME_ID_TWICE.test.ts # test 4 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT_CONCURRENT.test.ts # test 5 of 6"
                                sh "CI=true npm test src/test/ACCESS_WEBSOCKET.test.ts # test 6 of 6"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT.test.ts # test 1 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_KNOWN_SESSION.test.ts # test 2 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_NO_CLIENT_FEATURE.test.ts # test 3 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_SAME_ID_TWICE.test.ts # test 4 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT_CONCURRENT.test.ts # test 5 of 6"
                                sh "CI=true npm test src/test/ACCESS_WEBSOCKET.test.ts # test 6 of 6"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT.test.ts # test 1 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_KNOWN_SESSION.test.ts # test 2 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_NO_CLIENT_FEATURE.test.ts # test 3 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_SAME_ID_TWICE.test.ts # test 4 of 6"
                                sh "CI=true npm test src/test/ACCESS_CARTA_DEFAULT_CONCURRENT.test.ts # test 5 of 6"
                                sh "CI=true npm test src/test/ACCESS_WEBSOCKET.test.ts # test 6 of 6"
                            }
                        }
                        }
                    }
                }    
            }
        }
        stage('ICD tests: file-browser') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/GET_FILELIST.test.ts # test 1 of 9"
                                sh "CI=true npm test src/test/GET_FILELIST_ROOTPATH_CONCURRENT.test.ts # test 2 of 9"
                                sh "CI=true npm test src/test/FILETYPE_PARSER.test.ts # test 3 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS.test.ts # test 4 of 9"
                                sh "CI=true npm test src/test/FILEINFO_CASA.test.ts # test 5 of 9"
                                sh "CI=true npm test src/test/FILEINFO_HDF5.test.ts # test 6 of 9"
                                sh "CI=true npm test src/test/FILEINFO_MIRIAD.test.ts # test 7 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS_MULTIHDU.test.ts # test 8 of 9"
                                sh "CI=true npm test src/test/FILEINFO_EXCEPTIONS.test.ts # test 9 of 9"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/GET_FILELIST.test.ts # test 1 of 9"
                                sh "CI=true npm test src/test/GET_FILELIST_ROOTPATH_CONCURRENT.test.ts # test 2 of 9"
                                sh "CI=true npm test src/test/FILETYPE_PARSER.test.ts # test 3 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS.test.ts # test 4 of 9"
                                sh "CI=true npm test src/test/FILEINFO_CASA.test.ts # test 5 of 9"
                                sh "CI=true npm test src/test/FILEINFO_HDF5.test.ts # test 6 of 9"
                                sh "CI=true npm test src/test/FILEINFO_MIRIAD.test.ts # test 7 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS_MULTIHDU.test.ts # test 8 of 9"
                                sh "CI=true npm test src/test/FILEINFO_EXCEPTIONS.test.ts # test 9 of 9"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd" 
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/GET_FILELIST.test.ts # test 1 of 9"
                                sh "CI=true npm test src/test/GET_FILELIST_ROOTPATH_CONCURRENT.test.ts # test 2 of 9"
                                sh "CI=true npm test src/test/FILETYPE_PARSER.test.ts # test 3 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS.test.ts # test 4 of 9"
                                sh "CI=true npm test src/test/FILEINFO_CASA.test.ts # test 5 of 9"
                                sh "CI=true npm test src/test/FILEINFO_HDF5.test.ts # test 6 of 9"
                                sh "CI=true npm test src/test/FILEINFO_MIRIAD.test.ts # test 7 of 9"
                                sh "CI=true npm test src/test/FILEINFO_FITS_MULTIHDU.test.ts # test 8 of 9"
                                sh "CI=true npm test src/test/FILEINFO_EXCEPTIONS.test.ts # test 9 of 9"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: animator') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ANIMATOR_DATA_STREAM.test.ts # test 1 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_NAVIGATION.test.ts # test 2 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_PLAYBACK.test.ts # test 3 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR_MATCH.test.ts # test 4 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR.test.ts # test 5 of 5"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ANIMATOR_DATA_STREAM.test.ts # test 1 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_NAVIGATION.test.ts # test 2 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_PLAYBACK.test.ts # test 3 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR_MATCH.test.ts # test 4 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR.test.ts # test 5 of 5"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/ANIMATOR_DATA_STREAM.test.ts # test 1 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_NAVIGATION.test.ts # test 2 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_PLAYBACK.test.ts # test 3 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR_MATCH.test.ts # test 4 of 5"
                                sh "CI=true npm test src/test/ANIMATOR_CONTOUR.test.ts # test 5 of 5"
                            }
                        }
                        }
                    }
                }     
            }
        }
        stage('ICD tests: contour') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA_NAN.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/CONTOUR_DATA_STREAM.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA_NAN.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/CONTOUR_DATA_STREAM.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/CONTOUR_IMAGE_DATA_NAN.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/CONTOUR_DATA_STREAM.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: region statistics') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_STATISTICS_RECTANGLE.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_ELLIPSE.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_POLYGON.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_STATISTICS_RECTANGLE.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_ELLIPSE.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_POLYGON.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_STATISTICS_RECTANGLE.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_ELLIPSE.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/REGION_STATISTICS_POLYGON.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: region manipulation') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_REGISTER.test.ts # test 1 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_INFO.test.ts # test 2 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_INTERNAL.test.ts # test 3 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXPORT.test.ts # test 4 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXCEPTION.test.ts # test 5 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_EXPORT.test.ts # test 6 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_EXPORT.test.ts # test 7 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_DOS.test.ts # test 8 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXCEPTION.test.ts # test 9 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXPORT.test.ts # test 10 of 10"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_REGISTER.test.ts # test 1 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_INFO.test.ts # test 2 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_INTERNAL.test.ts # test 3 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXPORT.test.ts # test 4 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXCEPTION.test.ts # test 5 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_EXPORT.test.ts # test 6 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_EXPORT.test.ts # test 7 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_DOS.test.ts # test 8 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXCEPTION.test.ts # test 9 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXPORT.test.ts # test 10 of 10"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/REGION_REGISTER.test.ts # test 1 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_INFO.test.ts # test 2 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_INTERNAL.test.ts # test 3 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXPORT.test.ts # test 4 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_IMPORT_EXCEPTION.test.ts # test 5 of 10"
                                sh "CI=true npm test src/test/CASA_REGION_EXPORT.test.ts # test 6 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_EXPORT.test.ts # test 7 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_DOS.test.ts # test 8 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXCEPTION.test.ts # test 9 of 10"
                                sh "CI=true npm test src/test/DS9_REGION_IMPORT_EXPORT.test.ts # test 10 of 10"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: cube histogram') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_HDF5.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_CANCELLATION.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_HDF5.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_CANCELLATION.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM.test.ts # test 1 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_HDF5.test.ts # test 2 of 3"
                                sh "CI=true npm test src/test/PER_CUBE_HISTOGRAM_CANCELLATION.test.ts # test 3 of 3"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: spatial profiler') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE_NaN.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE_NaN.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/CURSOR_SPATIAL_PROFILE_NaN.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: raster tiles') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CHECK_RASTER_TILE_DATA.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/TILE_DATA_REQUEST.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CHECK_RASTER_TILE_DATA.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/TILE_DATA_REQUEST.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/CHECK_RASTER_TILE_DATA.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/TILE_DATA_REQUEST.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: spectral line query') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY_INTENSITY_LIMIT.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY_INTENSITY_LIMIT.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY.test.ts # test 1 of 2"
                                sh "CI=true npm test src/test/SPECTRAL_LINE_QUERY_INTENSITY_LIMIT.test.ts # test 2 of 2"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: moments') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CANCEL.test.ts # test 1 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CASA.test.ts # test 2 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_EXCEPTION.test.ts # test 3 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_FITS.test.ts # test 4 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_HDF5.test.ts # test 5 of 7"
                                sh "# skipping CI=true npm test src/test/MOMENTS_GENERATOR_PROFILE_STREAM.test.ts # test 6 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_SAVE.test.ts # test 7 of 7"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CANCEL.test.ts # test 1 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CASA.test.ts # test 2 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_EXCEPTION.test.ts # test 3 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_FITS.test.ts # test 4 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_HDF5.test.ts # test 5 of 7"
                                sh "# skipping CI=true npm test src/test/MOMENTS_GENERATOR_PROFILE_STREAM.test.ts # test 6 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_SAVE.test.ts # test 7 of 7"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CANCEL.test.ts # test 1 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_CASA.test.ts # test 2 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_EXCEPTION.test.ts # test 3 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_FITS.test.ts # test 4 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_HDF5.test.ts # test 5 of 7"
                                sh "# skipping CI=true npm test src/test/MOMENTS_GENERATOR_PROFILE_STREAM.test.ts # test 6 of 7"
                                sh "CI=true npm test src/test/MOMENTS_GENERATOR_SAVE.test.ts # test 7 of 7"
                            }
                        }
                        }
                    }
                }
            }
        }
        stage('ICD tests: resume') {
            parallel {
                stage('CentOS7') {
                    agent {
                        label "centos7-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "centos7-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/RESUME_CATALOG.test.ts # test 1 of 4"
                                sh "CI=true npm test src/test/RESUME_CONTOUR.test.ts # test 2 of 4"
                                sh "CI=true npm test src/test/RESUME_IMAGE.test.ts # test 3 of 4"
                                sh "CI=true npm test src/test/RESUME_REGION.test.ts # test 4 of 4"
                            }
                        }
                        }
                    }
                }
                stage('Ubuntu') {
                    agent {
                        label "ubuntu-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "ubuntu-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/RESUME_CATALOG.test.ts # test 1 of 4"
                                sh "CI=true npm test src/test/RESUME_CONTOUR.test.ts # test 2 of 4"
                                sh "CI=true npm test src/test/RESUME_IMAGE.test.ts # test 3 of 4"
                                sh "CI=true npm test src/test/RESUME_REGION.test.ts # test 4 of 4"
                            }
                        }
                        }
                    }
                }
                stage('MacOS') {
                    agent {
                        label "macos-1"
                    }
                    steps {
                        catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE')
                        {
                        sh "export PATH=/usr/local/bin:$PATH"
                        dir ('build') {
                            unstash "macos-1_carta_backend_icd"
                            sh "./run.sh # run carta_backend in the background"
                            dir ('carta-backend-ICD-test') {
                                sh "CI=true npm test src/test/RESUME_CATALOG.test.ts # test 1 of 4"
                                sh "CI=true npm test src/test/RESUME_CONTOUR.test.ts # test 2 of 4"
                                sh "CI=true npm test src/test/RESUME_IMAGE.test.ts # test 3 of 4"
                                sh "CI=true npm test src/test/RESUME_REGION.test.ts # test 4 of 4"
                            }
                        }
                        }
                    }
                }
            }
        }
    }
}
