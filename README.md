# 로그인 기반 채팅 프로그램<br>
## EW32 라이브러리를 활용한 클라이언트 - 서버 간의 소켓 통신 채팅 프로그램
### 진행 기간
> 24.4.7 ~ 24.4.21

### 사용 언어
![Langauge:C](https://img.shields.io/badge/Language-C-red) ![platform:Visual Studio](https://img.shields.io/badge/Platform-Visual_Studio-red)
<br>

### 진행한 챕터
<img src="03/Advanced_chatting_program.PNG" alt="Intro Screen" width="400px" height="300px"><img src="03/Server.PNG" alt="Intro Screen" width="400px" height="300px"><img src="03/client.PNG" alt="Intro Screen" width="400px" height="300px">
<br>

### 기능
- 공통적인 부분은 대화창의 내용을 계속 스크롤하면서 보는 것이 귀찮을 수 있으니 대화 내용 삭제
- 클라이언트 같은 경우 음악 재생 및 중지, 실습 화면 캡처, 매크로를 활용한 프로그램 접속 추가, 시계 표시
- 서버 같은 경우 정보 변경 시 아이디 미입력 뿐만 아니라 비밀번호 미입력도 리스트 박스에 표시
- 클라이언트에도 서버 접속 시 아이디/패스워드 미입력, 잘못된 아이디/패스워드 입력에도 서버 접속 x
=> 메시지 박스를 각기 다르게 두어(접속 성공 -> MB_ICONINFORMATION, 실패 -> MB_ICONERROR) 표시
- 클라이언트 맨 하단 리스트 박스를 추가로 생성하여 사용자가 입력한 키에 대한 후킹 데이터를 표시할 수 있도록 함
- 클라이언트 상단 오른쪽에 이미지 애니메이션 표시

### 문제점 및 해결
- 아이디가 중복되어 있는 사용자가 있다면 이벤트 창에 표시되었음
- 이와 같은 로직의 코드를 똑같이 사용해서 비밀번호가 중복된 사용자도 가려내기 위해서 작성
하지만 계속해서 이벤트 창에 표시가 되어 있지 않았음
- 문제의 코드를 분석해 보니 다음과 같았음







- CheckStringThatMakesUpId(pw)의 코드는 아이디의 유효성을 체크하는 부분
=> 비밀번호는 숫자든 영문자 + 숫자 + 특수 문자의 조합이든 크게 중요하지 않으므로 제거
- 신규 유저의 비밀번호와 기존 유저의 비밀번호를 비교하여 중복성 방지
=> pw_len != p_user->info.password_len 에서 pw_len == p_user->info.password_len으로 변경
- 신규 유저라면 입력된 그대로 비밀번호가 에디트 컨트롤에 등록되고 에디트 컨트롤에 복사됨
- 아이디는 입력한 상태이지만 비밀번호는 입력하지 않으면 이벤트 리스트 박스에 미입력된 상태라고 표시

