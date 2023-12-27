
fn main() {

    let mut howdy: String = "Howdy ".to_string();
    let parter: String = "Partner".to_string();

    howdy.push_str(&parter);
    println!("{}", howdy);

}