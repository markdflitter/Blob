pipeline {
	agent any
		stages {
			stage ('Build') {
				steps {
					echo 'Building...'
					sh "cmake ."
					sh "make"
					echo 'Done.'
				}
			}

			stage ('Test') {
				steps {
					echo 'Testing...'
sh "ctest .. --no-compress-output -T test || /bin/true"
sh "xsltproc CTest2JUnit.xsl Testing/`head -n 1 < Testing/TAG`/Test.xml > JUnitTestResults.xml"
					echo 'Done.'
				}
			}
		}
	post {
		always {
			junit 'JUnitTestResults.xml'
		 }
	 }
}
