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

## Note