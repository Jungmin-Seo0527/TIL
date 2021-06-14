# TIL

## 2. 스프링 핵심 원리 이해1 - 예제 만들기

### 2-1. 프로젝트 생성

#### build.gradle

```
plugins {
	id 'org.springframework.boot' version '2.5.1'
	id 'io.spring.dependency-management' version '1.0.11.RELEASE'
	id 'java'
}

group = 'hello'
version = '0.0.1-SNAPSHOT'
sourceCompatibility = '11'

repositories {
	mavenCentral()
}

dependencies {
	implementation 'org.springframework.boot:spring-boot-starter'
	testImplementation 'org.springframework.boot:spring-boot-starter-test'
}

test {
	useJUnitPlatform()
}

```

* 우선 순수 자바에서 부터 스프링까지 점진적으로 발전시키기 때문에 시작은 어떤것도 의존하지 않고 시작한다.
* Gradle 대신에 IntelliJ가 자바 직접 실행하도록 설정 변경

### 2-2. 비즈니스 요구사항과 설계

* 회원
    * 회원을 가입하고 조회할 수 있다.
    * 회원은 일반과 VIP 두 가지 등급이 있다.
    * 회원 데이터는 자체 DB를 구축할 수 있고, 외부 시스템과 연동할 수 있다.(미확정)

* 주문과 할인 정책
    * 회원은 상품을 주문할 수 있다.
    * 회원 등급에 따라 할인 정책을 적용할 수 있다.
    * 할인 정책은 모든 VIP는 1000원을 할인해주는 고정 금액 할인을 적용해달라.(나중에 변경 될 수 있다.)
    * 할인 정책은 변경 가능성이 높다. 회사의 기본 할인 정책을 아직 정하지 못했고, 오픈 직전까지 고민을 미루고 싶다. 최악의 경우 할인을 적용하지 않을 수 도 있다.(미확정)

요구사항을 보면 회원 데이터, 할인 정책 같은 부분은 지금 결정하기 어려운 부분이다. 그렇다고 이런 정책이 결정될 때 까지 개발을 무기한 기다릴 수 도 없다. 우리는 앞에서 배운 객체 지향 설계 방법이 있지
않은가!    
인터페이스를 만들고 구현체를 언제든지 갈아끼울 수 있도록 설계하면 된다.

### 2-3. 회원 도메인 설계

* 회원 도메인 요구사항
    * 회원을 가입하고 조회할 수 있다.
    * 회원은 일반과 VIP 두 가지 등급이 있다.
    * 회원 데이터는 자체 DB를 구출할 수 있고, 외부 시스템과 연동할 수 있다.(미확정)


* 회원 도메인 협력 관계
  ![](https://i.ibb.co/jJp5VqK/bandicam-2021-06-14-11-14-52-338.jpg)

* 회원 클래스 다이어그램
  ![](https://i.ibb.co/rMFPrYV/bandicam-2021-06-14-11-16-12-970.jpg)

* 회원 객체 다이어그램
  ![](https://i.ibb.co/8Bb4RfZ/bandicam-2021-06-14-11-16-55-936.jpg)

### 2-4. 회원 도메인 개발

#### 회원 엔티티

##### Grade.java - 회원 등급

* `src/main/java/hello/core1/member/Grade.java`

```java
package hello.core1.member;

public enum Grade {
    BASIC,
    VIP
}
```

##### Member.java - 회원 엔티티

* `src/main/java/hello/core1/member/Member.java`

```java
package hello.core1.member;

public class Member {

    private Long id;
    private String name;
    private Grade grade;

    public Member(Long id, String name, Grade grade) {
        this.id = id;
        this.name = name;
        this.grade = grade;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Grade getGrade() {
        return grade;
    }

    public void setGrade(Grade grade) {
        this.grade = grade;
    }
}

```

#### 회원 저장소

##### MemberRepository.java - 회원 저장소 인터페이스

* `src/main/java/hello/core1/member/MemberRepository.java`

```java
package hello.core1.member;

public interface MemberRepository {

    void save(Member member);

    Member findById(Long memberId);
}

```

##### MemoryMemberRepository.java - 메모리 회원 저장소 구현체

* `src/main/java/hello/core1/member/MemoryMemberRepository.java`

```java
package hello.core1.member;

import java.util.HashMap;
import java.util.Map;

public class MemoryMemberRepository implements MemberRepository {

    private static Map<Long, Member> store = new HashMap<>();

    @Override
    public void save(Member member) {
        store.put(member.getId(), member);
    }

    @Override
    public Member findById(Long memberId) {
        return store.get(memberId);
    }
}

```

데이터베이스가 아직 확정이 안되었다. 그래도 개발은 진행해야 하니 가장 단순한, 메모리 회원 저장소를 구현해서 우선 개발을 진행한다.

> 참고    
> `HashMap`은 동시성 이슈가 발생할 수 있다. 이런 경우 `ConcurrentHashMap`을 이용하자.

#### 회원 서비스

##### MemberService.java - 회원 서비스 인터페이스

* `src/main/java/hello/core1/member/MemberService.java`

```java
package hello.core1.member;

public interface MemberService {

    void join(Member member);

    Member findMember(Long memberId);
}

```

##### MemberServiceImpl.java - 회원 서비스 구현체

* `src/main/java/hello/core1/member/MemberServiceImpl.java`

```java
package hello.core1.member;

public class MemberServiceImpl implements MemberService {

    private final MemberRepository memberRepository = new MemoryMemberRepository();

    @Override
    public void join(Member member) {
        memberRepository.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberRepository.findById(memberId);
    }
}

```

## Note