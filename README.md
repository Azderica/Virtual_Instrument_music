# systemprograming
for system programming

프로젝트명: Virtual Music

1. 개요 
터미널 상에서 연주할 수 있는 가상 피아노를 구현합니다. 피아노악기 연주를 할 수 있게 61개의 피아노 음을 넣어, 쓰레드를 통한 동시재생을 구현합니다. 또한 악보 출력 및 프로그램을 통해 연주한 음악을 녹음하는 기능과, 저장된 음악을  재생하는 기능을 제공합니다.

2. 전체 시스템 구성 (보드 및 외부 PC 등 전체 외부 연결 구조 그림) 
키보드(Input) - PC(Terminal) - 스피커, 모니터(Output)

3. 시스템 디자인 (유스케이스 다이어그램: 사용사례 및 액터 표현) 
       
4. 시스템 내부 구조 (클래스 다이어그램: 시스템 내 모듈 구조) 
  
5. 팀원 구성 및 역할 (각 팀원별 이름, 학과, 학년 및 역할(맡은 모듈 등)) 
IT대학 컴퓨터학부 2015117629 김 지수
IT대학 컴퓨터학부 2015117532 박 명훈

6. 시스템 상세 설계  (주요 기능에 대한 구현 알고리즘)
  6.1 피아노 연주 (프로그램이 실행되는 시퀀스 다이어그램 또는 상태 다이어그램) 
  6.2 피아노 녹음 (프로그램이 실행되는 시퀀스 다이어그램 또는 상태 다이어그램) 
  6.3 녹음한 연주 재생(프로그램이 실행되는 시퀀스 다이어그램 또는 상태 다이어그램) 

7. 실행 결과 (실행 결과 화면 캡처, 시연 동영상 url) 

8. GitHub 업로드 결과 (GitHub url, 업로드 현황, 팀원별 활동 현황 (Insights) 등 캡처) 
   GitHub : https://github.com/Azderica/Virtual_Instrument_music.git
   Youtube : https://youtu.be/NJOvJhJKj6s

9. 고찰 (구현을 하면서 어려웠던 점 및 느낀점) 
   9.1. 구현 과정에서 어려웠던 점
	피아노 음을 키보드 입력에 따라 재생하기 위해서, 각 음을 하나하나 3초 길이의 wav 파일로 분		리하여 저장하는 과정에서 시간이 많이 걸렸습니다. 또한 피아노 연주를 녹음하는 기능을 구현하		기 위해서, 처음에는 입력을 1/10초 단위로 배열에 저장하여 재생하는 방식으로 녹음을 구현하고		자 하였으나, 구현 방식에 한계가 있고 정확도가 떨어져 microsecond단위로 구조체 배열에 		input과 경과시간을 저장하는 방식으로 구현하였습니다.  

10. 참고자료 (참고한 책 또는 사이트) 
->시스템 프로그래밍 강의자료
->http://bmfrog.tistory.com (녹음 기능 구현을 위한 time 관련 함수 구현에 참고)
->https://freesound.org/ (피아노 음 자료 사용)
