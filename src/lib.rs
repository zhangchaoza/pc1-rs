include!(concat!(env!("OUT_DIR"), "/bindings_pc1_128.rs"));
include!(concat!(env!("OUT_DIR"), "/bindings_pc1_256.rs"));

pub fn pc1_128_encrypt(key: &[u8], plain: &[u8], cipher: &mut [u8], block_length: usize) {
    unsafe {
        pc1_128_encrypt_block(
            key.as_ptr(),
            plain.as_ptr(),
            cipher.as_mut_ptr(),
            block_length,
        );
    }
}

pub fn pc1_128_decrypt(key: &[u8], cipher: &[u8], plain: &mut [u8], block_length: usize) {
    unsafe {
        pc1_128_decrypt_block(
            key.as_ptr(),
            cipher.as_ptr(),
            plain.as_mut_ptr(),
            block_length,
        );
    }
}

pub fn pc1_256_encrypt(key: &[u8], plain: &[u8], cipher: &mut [u8], block_length: usize) {
    unsafe {
        pc1_256_encrypt_block(
            key.as_ptr(),
            plain.as_ptr(),
            cipher.as_mut_ptr(),
            block_length,
        );
    }
}

pub fn pc1_256_decrypt(key: &[u8], cipher: &[u8], plain: &mut [u8], block_length: usize) {
    unsafe {
        pc1_256_decrypt_block(
            key.as_ptr(),
            cipher.as_ptr(),
            plain.as_mut_ptr(),
            block_length,
        );
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn pc1_128_works() {
        let key = b"0123456789ABCDEF";
        let plain = b"hello";

        let mut cipher = vec![0u8; plain.len()];
        pc1_128_encrypt(key, plain, &mut cipher, plain.len());
        assert_eq!(cipher, [0x8b, 0xa5, 0xf3, 0x4d, 0xc5]);

        let mut plain2 = vec![0u8; plain.len()];
        pc1_128_decrypt(key, &cipher, &mut plain2, cipher.len());
        assert_eq!(plain2, *plain);
    }

    #[test]
    fn pc1_256_works() {
        let key = b"0123456789ABCDEF0123456789ABCDEF";
        let plain = b"hello";

        let mut cipher = vec![0u8; plain.len()];
        pc1_256_encrypt(key, plain, &mut cipher, plain.len());
        assert_eq!(cipher, [0x4b, 0x41, 0xd6, 0x0f, 0xfd]);

        let mut plain2 = vec![0u8; plain.len()];
        pc1_256_decrypt(key, &cipher, &mut plain2, cipher.len());
        assert_eq!(plain2, *plain);
    }
}
