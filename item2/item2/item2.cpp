//항목 2(p53): #define을 쓰려거든 const, enum, inline을 떠올리자


#define ASPECT_RATIO 1.653
//컴파일러에 넘어가기 전에 선행 처리자가 숫자 상수로 바꾸어 버린다. 그래서 ASPECT_RATIO라는 이름이 컴파일러가 쓰는 기호 테이블에 들어가지 않는다.
//숫자 상수로 대체된 코드에서 컴파일 에러가 발생하면 헷갈릴 여지가 있다.

//이 문제의 해결법은 매크로 대신 상수를 쓰는 것이다.
const double AspectRatio = 1.653;
//위의 예제처럼 상수가 부동소수점 실수 타입일 경우에는 컴파일 후 최종 코드의 크기가 #define을 썼을 때보다 작게 나올 수 있다.

//#define을 상수로 교체할 시 주의점
//1. 상수 포인터를 정의하는 경우. 상수 정의는 대개 헤더 파일에 넣는 것이 상례이므로 포인터는 const 선언 해줘야 하며, 보통 포인터가 가리키는 대상까지 const로 선언한다.
const char * const authorName = "Scott Meyers";
//문자열 상수를 쓸때 char*를 쓰는것보다 string 객체가 대체적으로 사용하기 괜찮다.
const std::string authorName("Scott Meyers");

//2. 클래스 멤버로 상수를 정의하는 경우
class GamePlayer {
private:
	static const int NumTurns = 5;	//상수 선언
	int scores[NumTurns];			//상수를 사용하는 부분
};
//사용하고자 하는 것에 대해 '정의'가 마련되어 있어야 하는 게 보통이지만, 정적 멤버로 만들어지는 정수류 타입의 클래스 내부상수는 예외다.
//클래스 상수의 주소를 구하거나 컴파일러가 정의를 요구할 경우 별도의 정의를 제공해야 한다.
const int GamePlayer::NumTurns;	//NumTurns의 정의
//클래스 상수의 정의는 구현 파일에 작성한다. 정의에는 상수의 초기값이 있으면 안 되는데, 클래스 상수의 초기값은 해당 상수가 선언된 시점에서 바로 주어지기 때문이다.

//위의 문법이 먹히지 않는 컴파일러를 쓸 때는, 초기값을 상수 '정의' 시점에 주도록 한다.
class CostEstimate {
private:
	static const double FudgeFactor;	//정적 클래스 상수의 선언, 헤더 파일에 작성한다.
};

const double CostEstimate::FudgeFactor = 1.35;	//정적 클래스 상수의 정의, 구현 파일에 작성한다.

//위의 예제의 한 가지 예외가 있다면 해당 클래스를 컴파일하는 도중에 클래스 상수의 값이 필요할 때(컴파일 과정에서 scores 배열의 크기를 알아야할때)이다.
//enum hack 기법: 'enumerator타입의 값은 int가 놓일 곳에도 쓸 수 있다'를 이용한다.
class GamePlayer {
private:
	enum { NumTurns = 5 };	//NumTurns를 5에 대한 기호식 이름으로 만든다.
	int scores[NumTurns];
};

//#define 지시자의 또 다른 오용 사례는 매크로 함수이다.
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b));

int a = 5, b = 0;
CALL_WITH_MAX(++a, b);			//a가 두번 증가
CALL_WITH_MAX((++a, b + 10));	//a가 한번 증가
//비교 결과에 따라 a가 달라짐 f((++a) > (b) ? (++a) : (b));

//인라인 함수에 대한 템플릿으로 해결한다.
template<typename T>
inline void callWithMax(const T& a, const T& b) {
	f(a > b ? a : b);
}