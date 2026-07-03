document.addEventListener('DOMContentLoaded', ()=>{
  const copyBtn = document.getElementById('copyBtn');
  if(copyBtn){
    copyBtn.addEventListener('click', ()=>{
      const code = document.getElementById('code-sample').innerText;
      navigator.clipboard.writeText(code).then(()=>{
        copyBtn.textContent='Đã sao chép';
        setTimeout(()=>copyBtn.textContent='Sao chép đoạn mã',2000);
      });
    });
  }
  // Smooth scroll for internal links
  document.querySelectorAll('a[href^="#"]').forEach(a=>{
    a.addEventListener('click', e=>{
      e.preventDefault();
      const id = a.getAttribute('href');
      const el = document.querySelector(id);
      if(el) el.scrollIntoView({behavior:'smooth',block:'start'});
    });
  });
});
