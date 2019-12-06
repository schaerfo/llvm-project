// RUN: %check_clang_tidy %s modernize-use-override %t -- \
// RUN:   -config="{CheckOptions: [{key: modernize-use-override.AllowVirtualAndOverride, value: 1}]}"

struct Base {
  virtual ~Base();
  virtual void a();
};

struct Simple : public Base{
  virtual ~Base();
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: prefer using 'override' or (rarely) 'final' instead of 'virtual' [modernize-use-override]
  // CHECK-FIXES: {{^}}  ~Simple() override;
  virtual void a();
};
