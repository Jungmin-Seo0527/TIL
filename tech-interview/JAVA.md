# JAVA 예상 질문 모음

## 1. 데이터 타입은 무엇인가? 데이터 타입과 변수의 차이점은?

데이터 타입이란 해당 데이터가 메모리에 어떻게 저장되고, 프로그램에서 어떻게 처리되어야 하는지를 명시적으로 알려주는 것입니다. 기본적으로 기본형(primitive type)과 참조형(reference type)으로
나누어 집니다.

변수는 값을 저장할 수 있는 메모리 공간을 말합니다. 또한 변수에 값을 저장하려면 변수를 선언(생성)을 하고 값을 저장(초기화)해주어야 합니다.

## 2. 대표적인 Value Type과 Reference Type은?

value type으로는 booelan byte short char int long float double 이 있습니다.(primitive type이라고도 한다.)

reference type은 value type을 제외한 모든 타입이며, array, class, interface, enum이 있습니다.

> enum 열거형

## 3. char type과 string type이 존재하는 이유?

문자열을 저장할 수 있는 char 배열과 string 의 가장 큰 차이점은 수정의 가능 여부 입니다.        
char 타입은 수정이 가능하고, string은 수정이 불가능 합니다.(string은 read only)      
보통 +, - 연산으로 문자열을 변경하는데 이는 연산 결과의 문자열을 새로 할당하여 새로운 메모리 주소를 참조하는 방식으로 동작합니다.(기존의 문자열은 참조하는 객체가 없으므로 GC)

String은 객체로써 char보다 더 많은 기능을 제공할 수 있습니다.

## 4. value type과 reference type이 저장되는 stack과 heap영역에 대해 설명해 주세요.

stack은 지역 변수, 파리미터, 반환 값, 연산에서 이용되는 임시 값등을 저장하는 영역입니다.      
메소드를 호출하면 스택 영역이 생성되어 기본 타입 변수나 참조 타입 변수가 쌓이고 메소드가 종료되면 모두 pop을 수행하여 clear하게 만듭니다.

Heap영역은 `new`키워드로 싱성된 객체와 배열이 생성되는 영역입니다. GC가 동작하는 영역이기도 합니다.

기본적으로 primitive type은 스택 영역에 값 자체가 저장되는 반면에 reference type은 참조값이 stack에 저장됩니다.

## 5. 왜 stack 이라는 영역이 존재하는가? (모든 데이터를 heap에 저장하면 안됨?)

변수마다 생명 주기가 다르기 때문에 stack과 heap 영역으로 구분하여 관리합니다.        
stack에 존재하는 데이터는 메소드가 종료되는 시점에서 데이터를 반환합니다.     
만약 stack 없이 모두 heap에 데이터를 저장하면 메소드가 호출될 때마다 heap에 데이터가 쌓이기만 하기 때문에 리소스를 낭비하게 됩니다.(GC가 빈번하게 일어나나?)

## 6. .equals() 와 == 의 차이점

== 는 두 비교대상의 주소값을 비교합니다. 즉 두 변수가 동일한 변수를 가르키고 있으면 true   
equals는 두개의 대상의 값 자체를 비교합니다. 단 복수의 필드가 존재하는 사용자 정의 클래스에서는 오버라이딩이 필요합니다. (기본적으로 Reference type은 equals를 재정의 해놓음)

기본적으로 Object 객체에서 equals도 == 비교를 반환한다. 즉 두 대상이 같은 객체를 참조하고 있는지 판별한다. 다만 Primitive type이나 사용자 정의 클래스에서 equals를 오버라이딩 해서
비교 대상의 값 자체를 비교하도록 구현한다.(대표적으로 String이 있다.)

> equals 메소드의 비교 기준을 변경하는 방법은? equals()메소드를 오버라이딩 한다.       
> equals 메소드는 Object 객체에 구현되어 있는데 == 비교를 하고 있다.     
> 추가로 Comparable을 상속 받아서 compareTo()메소드를 오버라이딩 해도 equals()에는 영향이 없다.

## 7. .equals() 메소드는 2개의 객체가 같다는 것을 어떻게 비교하는가?

Object 객체에 정의되어 있는 equals 메소드는 기본적으로 == 비교입니다. 즉 주소값을 비교합니다.        
단 Object를 상속받은 클래스는 각자의 필드값에 맞게 equals을 오버라이딩 합니다.      
예를 들어 String은 같은 인덱스의 문자를 모두 비교해서 같은지 다른지 판별합니다.        
사용자 정의 클래스에서도 오버라리딩하여 재정의 할 수 있습니다.

## 8. `String a = "12345;` `String a = new String("12345);`의 차이점

`String a = "12345"`방식을 리터럴을 이용한 방식이라고 말합니다. 리터럴을 사용하면 string constanct pool, 혹은 intern pool이라고 하는 영역에 존재하고, new 를 통해
생성하면 heap영역에 존재합니다.

> 여기서 string pool과 동일한 문자열을 또다시 리터럴로 생성했을때 (`String aa = "12345";) intern()이라는 메소드를 실행하게 되는데 string pool에 저장하려는 문자열이 존재하는지 검색을 한다.      
> 존재한다면 존재하는 문자열의 참조값을 반환, 없으면 새로 생성해서 참조값을 반환한다.       
> 이렇게 되면 변수 a와 aa는 같은 문자열을 참조하는 형태가 된다. 단 new로 생성하면 그냥 heap에 문자열을 새로 생성한다.

## 9. `String a = "12345";`, `String b = new String("12345");`를 선언했을때 `a.equals(b)`의 결과값은? `a == b`의 결과값은?

`a.equals(b)`의 결과값은 true 데이터 내용이 같기 때문에     
`a == b`: false 주소값은 다르다. (a = string pool, b = heap)

## 10. 위 질문에서 `a = "54321";`로 바꾸고 `a.equals(b)'와 `a == b`의 결과값은?

false, false

## 11. 문자열 비교에서 `.equals()`메소드는 두 객체가 같은 메모리를 참조하고 있다는 것을 어떻게 알수 있는가? (내부 구조 설명)

* 내가 구현하는 equals, hashcode

```java
public class Temp {
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Parent parent = (Parent) o;
        return age == parent.age && Objects.equals(name, parent.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, age);
    }
}
```

```java
public class Objects {
    public static int hash(Object... values) {
        return Arrays.hashCode(values);
    }
}
```

```java
public class Arrays {
    public static int hashCode(Object a[]) {
        if (a == null)
            return 0;

        int result = 1;

        for (Object element : a)
            result = 31 * result + (element == null ? 0 : element.hashCode());

        return result;
    }
}
```

```java
public class String {
    public boolean equals(Object anObject) {
        if (this == anObject) {
            return true;
        }
        if (anObject instanceof String) {
            String aString = (String) anObject;
            if (!COMPACT_STRINGS || this.coder == aString.coder) {
                return StringLatin1.equals(value, aString.value);
            }
        }
        return false;
    }
}
```

기본적으로 == 비교를 하여 같은 객체를 참조하고 있는지 확인 후에 비교 대상의 데이터를 비교한다.

> 여기서 객체의 메모리 주소를 반환하는 hashCode()메소드를 언급한다면?? 다음 질문으로~

## 12. hashcode란 무엇인가?

실행중에(Runtime) 객체의 유일한 integer 값을 반환한다.      
Object 클래스에서는 heap에 저장된 객체의 메모리 주소를 반환한다.(항상 그런것은 아니다)
객체의 메모리 번지를 이용해서 해시코드를 만들어 리턴한다.

> 여기서 native 키워드를 볼수 있는데 이는 메소드가 JNI(Java Native Interface)라는 native code를 이용해 구현되었음을 의미한다.     
> native는 메소드에만 적용가능한 제어자로 C, C++등 Java가 아닌 언어로 구현된 부분을 JNI를 통해 Java에서 이용하고자 할 때 사용된다.

## 13. `.equals()`는 hashcode를 어떻게 이용해서 두 객체가 같은지 다른지 판단하는가?

동일한 객체, 정확하게 말하면 동일한 객체를 참조하고 있는 두 비교 대상은 동일한 메모리 주소를 가지고 있다는 것을 의미하므로 hashCode()의 반환값이 동일하다.

* Java 프로그램을 실행하는 동안 equals에 사용된 정보가 수정되지 않았다면, hashcode는 항상 동일한 정수값을 반환
* 두 객체가 equals()에 의해 동일하다면, 두 객체의 hashCode()값도 일치해야 한다.
* 두 객체가 equals()에 의해 동일하지 않다면, 두 객체의 hashCode()값은 일치하지 않아도 된다.
* hashCode()값이 같다고 무조건 equals()가 true일 필요는 없다. (단 다른 객체에서 동일한 hashcode()를 생성한다면 불이익을 받을 수 있다.) -> hashMap일때 문제가 발생하겠지

Map, Set 의 자료구조은 key의 유일성을 판별할 때 hashCode()를 확인한다. 따라서 동일한 객체는 동일한 hashCode()를 반환하는 것이 좋다.
**11번 질문에 hashcode()와 equals()구현체(오버라이딩) 작성함!!!**

## 14. hashcode를 이용해서 같은 객체인지를 비교하는 방법은?

hashcode는 주소값에 대한

## 15. 사용자 정의 클래스로 객체 인스턴스를 할당받았을 때 두 객체를 `equals()`로 같은지 판별하도록 어떻게 해야 하는가? (오버라이딩)

```java
public class Temp {
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Parent parent = (Parent) o;
        return age == parent.age && Objects.equals(name, parent.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, age);
    }
}
```

1. == 비교로 같은 객체를 참조하고 있는지 확인한다.
2. 같은 class 타입이 아니면 return false
3. 비교 대상이 될 맴버 변수를 비교한다.
4. 이때 hashCode() 메서드도 비교 기준을 설정하여 오버라이딩 해야 한다.
5. 이렇데 되면 같은 객체를 참조하지 않아도 그 데이터가 같으면 같은 hashcode, equals()=true가 된다.

## 16. 자바의 메모리 구조

## 17. `String a = "12345";`와 `String a = new String("12345");`방법중 어느 방법이 더 좋은가?

메모리 절약 차원에서 `String a = "12345"`이 더 좋을 것 같다.

## 18. string pool 이라고 들어본적이 있는가?

리터럴 방식으로 String 변수를 생성할 때 문자열이 저장되는 공간    
수정이 불가능

리터럴 방식으로 문자열을 생성할 때 `intern()`메소드가 실행되면서 이미 string pool에 동일한 문자열이 존재하는지 확인한다.

## 19. 맴버 변수와 지역 변수중 stack 영역에 저장되는 변수는?

지역변수

stack에는 지역변수, 파라미터, 임시 변수(메서드 내에서만 사용하는 변수), 반환값이 저장된다.

## 20. 맴버 변수와 지역 변수는 언제 메모리에서 해제 되는가?

맴버 변수는 더이상 자신을 참조하는 객체가 존재하지 않을때(heap)    
지역 변수는 해당 메소드가 종료될 때(stack)

## 21. heap영역에 저장되어 있는 변수는 언제 해제가 되는가?

stack에 존재하는 객체들중 누구도 참조하고 있지 않을 때

* Unreachable: stack 영역에서 도달할 수 없는 heap영역

## 22. 참조 타입의 주소는 어디에 저장이 되는가?

stack

## 23. 주소값은 왜 stack에 저장이 되는가?

인스턴스의 소멸 방법과 소멸 시점이 지역변수와는 다르기 때문

> 여기서 stack와 heap을 넘어서 JVM과 컴파일 과정으로 질문이 넘어갈 수 있다.
>
> stack
>
> Heap 영역에 생성된 Object 타입의 데이터의 참조값이 할당된다.   
> 원시타입의 데이터가 값과 함께 할당   
> 지역 변수들은 scope에 따른 visibility를 가진다.(다른 메소드가 접근 불가)   
> 각 thread는 자신만의 stack을 가진다.
>
> Heap    
> 긴 생명주기 (크기가 크고, 서로 다른 코드 블럭에서 공유)   
> 애플리케이션의 모든 메모리 중 stack을 제외한 나머지   
> 모든 Object 타입은 heap영역에 생성    
> 몇개의 쓰레드가 존재하든 Heap은 단 한개    
> Heap 영역에 있는 개체들을 가르키는 래퍼런스 변수가 stack에 존재한다.

## 24. 자료구조를 선택하는 기준은? 속도? 크기?

## 25. List, Set, Map의 차이점은?

중복 가능 여부

## 26. List와 Set, Map은 어떤 인터페이스를 구현하는가?

* List: ArrayList, LinkedList
* Set: HashSet, TreeSet, LinkedHashSet
* Map: HashMap, TreeMap, LinkedHashMap

## 27. `forEach`를 사용할 수 있는 자료구조는 어떠한 인터페이스를 상속받고 있는가?

Iterable

```java
public interface Iterable<T> {
    default void forEach(Consumer<? super T> action) {
        Objects.requireNonNull(action);
        for (T t : this) {
            action.accept(t);
        }
    }
}
```

`accept()` 메소드는 `Consumer`인터페이스에 정의되어 있따.

## 28. forEach를 사용할 때 다음 데이터를 얻기 위해서 호출되는 메소드는?

Consumer 인터페이스의 accept 메소드 실행

## 29. Iterator, Iterable의 차이점

## 30. Iterater를 상속받았을 때 구현해야 하는 메서드는?

## 31. 배열을 사이즈가 변경될 수 있는가?

## 32. 배열의 사이즈를 5개로 줄이려면 어떻게 해야 하는가?

## 33. 사이즈가 변경될 경우 어떠한 자료구조를 사용하는 것이 좋은가?

LinkedList

## 34. 동적(가변) 자료구조는 어떠한 것이 있는가?

List, LinkedList    
가변 자료구조는 그 크기가 계속 변할 수 있는 자료 구조   
이러먼 Map, Set도 모두 가능한 것 아닌가?

Array는 크기가 정해져 있다.

## 35. Generic 타입과 None Generic 타입의 자료구조의 차이점은?

## 37. 제너릭 타입의 자료구조는 어떠한 것이 있는가?

## 38. 제너릭 타입의 자료구조의 장점은? (Object 형을 저장하면 안되는가?)

## 39. ArrayList와 LinkedList의 차이점

포인터 유무 (각 노드가 다음 노드의 참조값을 가지고 있다.)

## 40. 위 질문의 두 자료구조중에 데이터를 순차적으로 찾을 때 가장 적합한 것은?

ArrayList   
임의의 인덱스로의 접근을 바로 할 수 있다.

## 41. 데이터를 빈번하게 추가 ,삭제할 때 가장 적합한 것은? 그 이유는?

LinkedList    
추가 삭제 이전, 이후의 노드의 포인터만 수정하면 O(1)로 수행 가능   
ArrayList는 새로 배열을 할당받아서 싹다 복사시켜야 한다.

## 42. ArrayList와 LinkedList에서 중간의 데이터를 삭제하면 내부적으로 어떠한 작업이 수행되는가?

ArrayList는 배열을 새로 할당해서 배열을 통채로 복사

LinkedList는 해당 노드만 제거하면 이전과 이후의 노드의 다음 노드에 대한 정보만 수정하면 된다.

## 43. ArrayList와 LinkedList중 검색 속도가 더 빠른 것은?

ArrayList

## 44. List에서 검색이 제일 느린 케이스는?

가장 마지막 인덱스에 원하는 정보가 위치했을 때????

## 45. LinkedList는 이전 Node, 이후 Node의 정보를 어떻게 알까?

이전 노드와 이후 노드의 참조값을 가지고 있다.

## 46. LinkedList는 중간에 데이터를 삽입하면 내부적으로 어떻게 동작하는가?

이전 노드는 해당 노드의 다음 노드를, 이후 노드는 해당 노드의 이전 노드를 가리키도록 한다.

## 47. List와 Set의 차이점은?

중복 가능 여부

## 48. Set이 중복을 허용하지 않는다면 내부적으로 동일하다는 것을 어떻게 확인하는가?

hashCode()    
해당 주소값을 해싱하여 유일한 정수값을로 반환한다. (오버라이딩 해서 재정의 가능)

## 49. Set에서 동일하다는 기준을 바꿀 수 있는가?

ㅇㅇ hashcode() 오버라이딩

equals()와 hashcode()는 항상 함께 오버라이딩 하는 것이 좋음

## 50. Map은 다른 자료구조와 무엇이 다른가?

다른 어떤 자료 구조??

set과 다른 점은 key값을 따로 가지고 있다는 점 (key에 대한 유일성을 가진다)    
List와 다른 점은 중복을 허용하지 않는다는 점

## 51. Map을 사용해야 하는 경우의 예

## 52. 알고있는 Map에는 어떤 것이 있는가?

HashMap, TreeMap, LinkedMap

> Map과 Set의 구현체중 HashMap, TreeMap, LinkedMap, HashSet, TreeSet, LinkedSet HashTable 은 알고 있는 것이 좋다.

## 53. HashTable과 HashMap의 차이점은?

* 키나 값에 null저장 가능 여부 (HaspMap은 가능, HashTable은 불가능)
* 여러 쓰레드 안전 여부 (HashMap은 불가능, HashTable은 가능)
* HashMap을 Thread safe하게 이용하려면 `Map m = Collections.symchronizedMap(new HashMap(...);`와 같이 선언해야 한다.

## 54. Hashing이란 무엇인가?

* 데이터의 효율적인 관리를 위해 임의의 길이의 데이터를 고정된 길이의 데이터로 매핑하는 것

## 55. Hash값이 중복될 수 있는데 Hash의 충돌을 피할 수 있는 방법은?

1. 체이닝: 연결리스트로 노드를 계속 추가한다. (추가에 대한 제한이 없지만 메모리 문제 발생 가능)
2. Open Addressing: 해시 함수로 얻은 주소가 아닌 다른 주소에 데이터를 저장할 수 있도록 허용(해당 키 값에 저장되어 있으면 다음 주소에 저장)
3. 선형 탐사: 정해진 고정 폭으로 옮겨 해시값의 중폭을 피한다.
4. 제곱 탐사: 정해진 고정 폭을 제곱수로 옮겨 해시값의 중복을 피한다.

## 56. Hashing을 하면 무엇이 좋은가?

검색 속도 O(1)

## 57. Queue와 Stack의 차이점은?

Queue는 선입 선출    
Stack은 후입 선출

## 58. Queue와 Stack을 사용하는 예

* 스택 활용 예시
    * 웹 브라우저 방문 기록(뒤로 가기)
    * 역순 문자열 만들기
    * 실행 취소(Undo)
    * 후위 표기법 계산
    * 수식의 괄호 검사(연산자 운선순위 표현을 위한 괄호 검사)
* 큐의 활용 예시
    * 우선순위가 같은 작업 예약(프린터의 인쇄 대기열)
    * 은행 업무
    * 콜센터 고객 대기 시간
    * 프로세스 관리
    * 너비 우선 탐색
    * 캐시 구현

## 59. Queue와 Stack은 내부적으로 어떤 자료구조를 사용하는가? 만약 Queue와 Stack을 직접 구현할 때 어떤 자료구조를 사용하는 것이 좋을까? 그 이유는?

* Queue는 LinkedList
* Stack는 Vector (Stack 구현체는 Vector를 상속 받는다.)
* 자바에서 Stack은 특이하게 인터페이스로 제공되지 않는데 이는 Collections 프레임 워크를 제공하기 전에 구현되어 서비스되었기 때문이라고 한다.

## 60. 만약에 인스턴스 메시지를 등록된 순서대로 발송한다고 하면 어떤 자료구조를 이용할까?

선입 선출을 보장하는 Queue 자료구조

## 61. 이때 배열이나 List를 사용하면 안되는가? Queue나 Stack을 이용한 것과의 차이점은?

List 인터페이스는 set이라는 메소드를 정의하고 있는데 특정 인덱스의 데이터값을 수정하는 인덱스이다.    
Stack과 Queue는 양 끝에서만 데이터의 삽입, 삭제가 보장되어야 한다. 따라서 List보다는 Queue, Stack을 사용하는 것이 적절하다.

## 62. Queue를 이용해서 선입선출로 메시지 발송이 되고 있는데 긴급 발송건이나 예약 발송건이 있으면 즉 우선순위가 존재할 때 어떤 자료구조를 사용해야 할까?

우선순위 큐

사용자 정의 클래스는 `Comparable`인터페이스를 상속 받은 후에 compareTo()메소드를 오버라이딩 한다.

## 63. Tree는 무엇이고 다른 자료구조와의 차이, 어떤 경우에 사용되는가?

* 대상 정보의 각 항목들을 계층적으로 연과되도록 구조화할 때 사용하는 비선형 자료구조
* 부모-자식 관계의 계층적 구조
* 그래프의 한 종류이며 사이클이 없다.
* 용어 정리
    * node: 트리를 구성하고 있는 각 요소
    * Edge: 트리를 구성하기 위해 노드와 노드를 연결하는 선
    * Root Node: 최상위 계층에 존재하는 노드
    * Level: 트리의 특정 깊이를 가지는 노드의 집합
    * Degree(차수): 하위 트리 갯수/간선 수 = 각 노드가 지닌 가지의 수
    * Terminal Node(=left node, 단말 노드): 하위에 다른 노드가 연결되어 있지 않은 노드
    * Internal Node(내부 노드, 비단말 노드): 단말 노드를 제외한 모든노드, 루르노드 포함


* 용도: 리눅스와 윈도우의 **파일시스템 구조**
* 데이터의 양이 많을 때 많이 사용

## 64. 이진트리 (B-Tree)란 무엇인가?

* 트리를 구성하는 노드들의 최대 차수가 2인 노드로 구성되는 트리
* 이진 트리의 레벨i에서 가질 수 있는 최대 노드의 수는 `2^i`이다. (i >= 0)
* 깊이가 k인 이진트리가 가질 수 있는 최대 노드의 수는 `2^k - 1`이다. (k >= 1)
* 완전 이진트리, 포화 이진트리, 전 이진 트리로 나눌 수 있다.
    * 완전 이진 트리: 왼쪽에서 오른쪽으로 채워진다. Heap
    * 전 이진 트리: 모든 노드가 0개 또는 2개의 자식 노드를 가진 트리(1개는 존재하지 않음)
    * 포화 이진 트리: 전 이진 트리면서 완전 이진 트리(완전한 모양)

## 65. 이진트리는 내부적으로 데이터가 어떻게 저장되는가?

* 기본적으로는 왼쪽 오른쪽 노드의 참조값을 가지고 있는 객체를 가진 노드로 구현 가능하다.(연결 리스트 사용)
* 하지만 일차원 배열로 쉽게 구현이 가능
* 0번이 아닌 1번 인덱스를 root로 하여 왼쪽 자식은 부모 인덱스 * 2, 오른쪽 자식은 부모 인덱스 * 2 + 1로 표현 가능
* 자식을 기준으로 자식 인덱스/2 = 부모 인덱스
* 단점
    * 빈 노드에 대해서 계속 사용하지 않기 때문에 메모리 낭비
    * 데이터의 삽입, 삭제 연산시 노드의 레벨 변경으로 인한 데이터의 이동 발생

## 66. 이진 검색 트리 (Binary Search Tree)는 무엇인가?

* 모든 노드가 자신의 왼쪽 서브트리에는 현재 노드보다 작은 키값이, 오른쪽 서브트리에는 현재 ㅈ노드보다 큰 값이 오는 규칙을 만족하는 이진트리
* 규칙
    * 이진 탐색 트리의 노드에 저장된 키는 유일하다.
    * 루트 노드의 키가 왼쪽 서브 트리를 구성하는 어떠한 노드의 키보다 크다.
    * 루트 노드의 키가 오른쪽 서브 트리를 구성하는 어떠한 노드의 키보다 작다.
    * 왼쪽과 오른쪽 서브트리오 이진 탐색 트리이다.
* 탐색 연산 시간 복잡도: `O(log n)`
* 비교적 삽입 삭제가 효율적인 자료구조
* 이진 탐색 트리는 편향 트리가 될 수 있다.
    * 저장 순서에 따라 계속 한 쪽으로만 노드가 추가되는 경우가 발생(`O(N)`)
    * 이를 해결하기 위해 **균형잡힌 이진검색트리**를 고안(레드블랙트리(TreeMap에서 사용됨), AVL트리)

## 67. 이진 검색 트리는 내부적으로 데이터가 어떻게 저장되는가?

현제 데이터가 저장될 수 있는 노드를 탐색하여 저장한다.

## 68. 이진 검색 트리에서 검색 속도가 가장 느린 케이스는?

편향된 트리(한쪽으로만 데이터가 계속 저장)

## 69. 동기화를 지원하는 자료구조는?

* CopyOnWriteArrayList
* HashTable
* ConcurrentHashMap
* Vector
* Stack


* ArrayList와 다르게 Vector는 동기화된 메소드로 구성되어 있다.
* 멀티 쓰레드가 동시에 이 메소드를 실행할 수 없고, 하나의 쓰레드가 실행을 완료해야 다른 쓰레드가 실행할 수 있다.
* Stack은 List 컬렉션 클래스의 Vector 클래스를 상속 받아, 전형적인 스택 메모리 구조의 클래스를 제공한다.

## 70. 동기화를 지원하는 자료구조는 왜, 언제 필요하는가?

* 멀티 쓰레드 환경에서 안전하게 객체를 추가, 삭제가 필요할 때

## 71. 멀티 쓰레드 환경에서 동기화를 지원하지 않으면 어떤 문제가 발생하는가?

* 추가 삭제가 안전하게 동작하지 않는다.

## 그리고 검색과 정렬 알고리즘...

## Reference

* [java 기본 타입 vs 참조 타입(feat. heap, stack 영역)](https://week-year.tistory.com/141)
* [[Java] equals와 hashCode 함수](https://mangkyu.tistory.com/101)
  [hashCode()와 equals()메서드는 언제 사용하고 왜 사용할까?](https://jisooo.tistory.com/entry/java-hashcode%EC%99%80-equals-%EB%A9%94%EC%84%9C%EB%93%9C%EB%8A%94-%EC%96%B8%EC%A0%9C-%EC%82%AC%EC%9A%A9%ED%95%98%EA%B3%A0-%EC%99%9C-%EC%82%AC%EC%9A%A9%ED%95%A0%EA%B9%8C)
* [Hash란 무엇인가?](https://jroomstudio.tistory.com/10)
* [Tree (이진트리, 이진탐색트리, Heap까지 설명)](https://velog.io/@adam2/TREE)
* [08-자료구조: 트리(Tree) - 일반 트리와 트리의 정의,일반트리의 구현](https://m.blog.naver.com/justkukaro/220548164184)
* [개발자 기술 면접 복기 3탄](https://surgach.tistory.com/70)
